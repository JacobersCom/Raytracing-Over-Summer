#pragma once

#include <limits>

class Interval
{
public:
	double min, max;

	Interval() : min(std::numeric_limits<double>::infinity()), max(-std::numeric_limits<double>::infinity()) {};

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

	double clamp(double x) const 
	{
		if (x < min) return min;
		if (x > max) return max;
		return x;
	}

	static const Interval empty, universe;
};

