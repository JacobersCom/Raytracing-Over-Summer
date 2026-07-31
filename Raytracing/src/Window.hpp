#pragma once

#include <Hittable.hpp>

class Camera
{
public:

	void Render(const Hittable& world);

private:

	void InitWindow();
	color RaySceneColor(const Ray& r, const Hittable& world) const;
};