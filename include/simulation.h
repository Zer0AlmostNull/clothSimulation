#include <vector>
#include <memory>

#include "render_context.h"
#include "types.h"

#include "vector2.hpp"

#pragma once


struct Point
{
	v2 position, prev_position;
	bool locked;

	Point(v2 _pos, v2 _prev_pos, bool _locked) : position(_pos), prev_position(_prev_pos), locked(_locked) {};
	Point(v2 _pos, bool _locked) : position(_pos), prev_position(_pos), locked(_locked) {};
};

struct Stick
{
	Point *a, *b;

	f32 length;

	Stick(Point& _a, Point& _b) : a(&_a), b(&_b)
	{
		length = sqrtf((_a.position.x - _b.position.x) * (_a.position.x - _b.position.x) +
			(_a.position.y - _b.position.y) * (_a.position.y - _b.position.y));
	}

	Stick& operator=(const Stick& other) {
		if (this != &other) {
			a = other.a;
			b = other.b;

			length = other.length;
		}
		return *this;
	}
};

class Simulation
{
	RenderContext& rc;
		
	std::vector<Stick> _sticks;
	std::vector<std::unique_ptr<Point>> _points;

public:
	Simulation(RenderContext&);

	void update(f32);
	void draw();

};