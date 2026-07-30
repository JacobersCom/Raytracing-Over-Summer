#pragma once

#include <Hittable.hpp>

#include <memory>
#include <vector>

class Hittable_List : public Hittable 
{
public:
	std::vector<std::shared_ptr<Hittable>> objects;

	Hittable_List() {};
	Hittable_List(std::shared_ptr<Hittable> _object) { add(_object); };

	void Clear() { objects.clear(); };

	void add(std::shared_ptr<Hittable> _object)
	{
		objects.push_back(_object);
	}

	bool hit(const Ray& r, double t_min, double t_max, hit_record& rec) const override
	{
		hit_record temp_rec;
		bool hit_anything = false;
		auto closest_so_far = t_max;

		for (const auto& object : objects)
		{
			//If we hit this object
			if (object->hit(r, t_min, t_max, temp_rec))
			{
				hit_anything = true;
				closest_so_far = temp_rec.t;
				rec = temp_rec;
			}
		}
		return hit_anything;
	}
};