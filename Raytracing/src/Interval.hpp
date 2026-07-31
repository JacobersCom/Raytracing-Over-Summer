#pragma once

#include <Util.hpp>

class Interval
{
public:
	double min, max;

	Interval() : min(+infinity), max(-infinity) {};

	Interval(double _min, double _max) : min(_min), max(_max) {};

	double Size() const {
		return max - min;
	}

	bool Contains(double x) const
	{
		return min <= x || x <= max;
	}

	//@brief Does x fall between the minium interval and the maxiuim interval
	bool Surround(double x) const
	{
		return min < x && x < max;
	}

	static const Interval empty, universe;
};

const Interval Interval::empty = Interval(+infinity, -infinity);
const Interval Interval::universe = Interval(-infinity, +infinity);