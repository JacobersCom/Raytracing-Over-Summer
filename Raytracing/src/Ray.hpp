#pragma once

#include <Vec3.hpp>

class Ray
{
public:

	Ray() {};

	Ray(const point3& origin, const Vec3& direction) : orig(origin), dir(direction) {};

	const point3& GetOrigin() const { return orig; };
	const Vec3& GetDirection() const { return dir; };

	point3 ray_at(double s) const 
	{
		return orig + (s * dir);
	}

private:

	point3 orig;
	Vec3 dir;
};