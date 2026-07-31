#pragma once

#include <Util.hpp>

struct hit_record
{
	point3 hit_point;
	Vec3 normal;
	double t;
	bool font_face;

	//NOTE:the outward_normal is assumed to be a unit vector
	void SetFaceNormal(const Ray& r, const Vec3& outward_normal)
	{
		//If the ray and normal point in the same direction, the ray is on the inside of the sphere
		//if the ray and normal point in different directions, the ray is on the outside of the sphere
		font_face = dot(r.GetDirection(), outward_normal) < 0;
		normal = font_face ? outward_normal : -outward_normal;
	}
};

class Hittable
{
public: 

	virtual ~Hittable() = default;

	//Just declaring a hit interface
	virtual bool hit(const Ray& r, Interval ray_t, hit_record& rec) const = 0;
};