#pragma once

#include <Hittable.hpp>

class Sphere : public Hittable
{
public:

	//fmax ensures the radius is never below 0
	Sphere(const point3& _center, double _radius) : center(_center), radius(std::fmax(0,_radius)) {};

	//Did we hit an object with a ray
	bool hit(const Ray& r, Interval ray_t, hit_record& rec) const override
	{
		Vec3 origin_to_sphere_center = center - r.GetOrigin();
		//Get the length of the direction because in the quadriate equation a is the dot producted of the ray's direction sqrted.
		auto a = r.GetDirection().length_sqrt();
		auto h = dot(r.GetDirection(), (center - r.GetOrigin()));
		auto c = origin_to_sphere_center.length_sqrt() - pow(radius, 2);
		auto discriminant = pow(h, 2) - a * c;

		if (discriminant < 0)
			return false;

		auto discriminant_sqrt = std::sqrt(discriminant);

		//Find the nearest root within a accpetable range

		//This is just the quadratic formula
		auto root = ((h - discriminant_sqrt) / a);
		if (!ray_t.Surround(root))
		{
			root = (h + discriminant_sqrt) / a;
			if (!ray_t.Surround(root))
				return false;
		}

		//T is where the ray hit the shpere
		rec.t = root;
		rec.hit_point = r.ray_at(rec.t);
		Vec3 outward_normal = (rec.hit_point - center) / radius;
		rec.SetFaceNormal(r, outward_normal);

		return true;
	}

private:

	point3 center;
	double radius;
};