#include "../SDL2/include/SDL_events.h"
#include "../SDL2/include/SDL_render.h"

#include "../include/simulation.h"
#include "math.h"

#include <algorithm>

// little drawing hack
void draw_line(SDL_Renderer* r, i16 x0, i16 y0, i16 x1, i16 y1);
void draw_rect_filled(SDL_Renderer* r, i16 x0, i16 y0, u16 width, u16 height);

Simulation::Simulation(RenderContext &_rc) : rc(_rc)
{

	v2 displacement(WND_WIDTH_HALF - (GRID_WIDTH-1) * POINTS_DIST *.5f, 
					-10);
	
	// create points
	_points.reserve(GRID_WIDTH * GRID_HEIGHT);

	for (u8 y = 0; y < GRID_HEIGHT; y++)
	{
		for (u8 x = 0; x < GRID_WIDTH; x++)
		{
			_points.push_back(std::make_unique <Point>(v2(x * POINTS_DIST, WND_HEIGHT - y * POINTS_DIST) + displacement, false));
		}
	}

	// lock some points
	(*_points[0]).locked = true;
	(*_points[GRID_WIDTH * .5]).locked = true;
	(*_points[GRID_WIDTH * .25]).locked = true;
	(*_points[GRID_WIDTH * .75]).locked = true;
	(*_points[GRID_WIDTH-1]).locked = true;


	// create sticks
	_sticks.reserve(2 * GRID_WIDTH * GRID_HEIGHT - GRID_WIDTH - GRID_HEIGHT);

	// horizontal
	for (u8 y = 0; y < GRID_HEIGHT; y++)
		for (u8 x = 0; x < GRID_WIDTH - 1; x++)
			_sticks.push_back(Stick(*_points[x + y*GRID_WIDTH], *_points[x + 1 + y * GRID_WIDTH]));

	// vertical
	for (u8 x = 0; x < GRID_WIDTH; x++)
		for (u8 y = 0; y < GRID_HEIGHT-1; y++)
			_sticks.push_back(Stick(*_points[x + y * GRID_WIDTH], *_points[x  + (y + 1) * GRID_WIDTH]));
	// diagal - add stiffness
	//for (u8 y = 0; y < GRID_HEIGHT-1; y++)
	//	for (u8 x = 0; x < GRID_WIDTH - 1; x++)
	//		_sticks.push_back(Stick(*_points[x + y * GRID_WIDTH], *_points[x + 1 + (y+1) * GRID_WIDTH]));


	// shuffle sticks to avoid jittering
	std::random_shuffle(_sticks.begin(), _sticks.end());
};

void Simulation::update(f32 deltaTime)
{
	
	// get mouse pos
	i32 mouse_x = 0, mouse_y = 0;

	u32 mouse_state = SDL_GetMouseState(&mouse_x, &mouse_y);
	mouse_y = WND_HEIGHT - mouse_y;


	// if mouse is clicked
	if (mouse_state & SDL_BUTTON(SDL_BUTTON_LEFT))
	{

		if (!(mouse_x < 0 || mouse_y < 0))
		{
			// collision check
			for (u16 i = 0; i < size(_sticks); i++)
			{
				v2 center = (_sticks[i].a->position + _sticks[i].b->position) / 2;

				if (sqrtf((center.x - mouse_x) * (center.x - mouse_x) + (center.y - mouse_y) * (center.y - mouse_y)) < _sticks[i].length / 2)
				{
					_sticks.erase(_sticks.begin() + i);
					break;
				}
			}
		}
	}
	

	// update points
	for (const auto& pointerPtr : _points)
	{
		Point& p = *pointerPtr;
		// dont move locked point
		if (p.locked) continue;

		v2 position_before_update = p.position;

		// apply velocity
		p.position += p.position - p.prev_position;

		// apply gravity
		p.position += v2(0, -1.0) * GRAVITY * deltaTime * deltaTime;

		p.prev_position = position_before_update;
	}


	const u16 NUM_ITERATION = 3;

	// update sticks
	for (u16 x = 0; x < NUM_ITERATION; x++)
	{
		for (auto& stick : _sticks)
		{
			v2 stick_center = (stick.a->position + stick.b->position) / 2;
			v2 stick_dir = (stick.a->position - stick.b->position).normalized();

			if (!stick.a->locked)
				stick.a->position = stick_center + stick_dir * stick.length / 2;
			if (!stick.b->locked)
				stick.b->position = stick_center - stick_dir * stick.length / 2;

		}
	}

	const int BOTTOM_MARGIN = -300;
	// prevent falling too far out of map
	for (int i = size(_sticks)-1; i>-1; i--)
	{		
		if (_sticks[i].a->position.y < BOTTOM_MARGIN && _sticks[i].b->position.y < BOTTOM_MARGIN)
			_sticks.erase(_sticks.begin() + i);
	}

}

void Simulation::draw()
{
	
	// const f32 point_size = 9;
	// set color to white
	//SDL_SetRenderDrawColor(rc.renderer, 255, 255, 255, 0);
	//for (const auto& pointerPtr : _points)
	//{
	//	const Point & p = *pointerPtr;
	//	draw_rect_filled(rc.renderer, p.position.x - point_size / 2, p.position.y + point_size / 2, point_size, point_size);
	//}
	
	// set color to red
	SDL_SetRenderDrawColor(rc.renderer, 255, 0, 0, 0);
	for (const Stick& s : _sticks)
	{
		draw_line(rc.renderer, s.a->position.x, s.a->position.y, s.b->position.x, s.b->position.y);
	}

}

void draw_line(SDL_Renderer* r, i16 x0, i16 y0, i16 x1, i16 y1)
{
	// flip y coords
	y0 = WND_HEIGHT - y0;
	y1 = WND_HEIGHT - y1;

	SDL_RenderDrawLine(r, x0, y0, x1, y1);
}

void draw_rect_filled(SDL_Renderer* r, i16 x0, i16 y0, u16 width, u16 height)
{
	// flip y coords
	y0 = WND_HEIGHT - y0;

	SDL_Rect rect{x0, y0, width, height};

	SDL_RenderFillRect(r, &rect);
}