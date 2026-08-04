#pragma once

#include <Hittable.hpp>
#include <Hittable_List.hpp>
#include <Sphere.hpp>

#include <SDL3/SDL.h>

#include <imgui.h>
#include <imgui_impl_sdl3.h>
#include <imgui_impl_sdlrenderer3.h>

class Application
{
public:
	

	void Initalization();
	void Update(class Hittable_List& world);
	
public:

	double aspect_ratio = 1.0;  // Ratio of image width over height
	int    image_w = 100;  // Rendered image width in pixel count
	int sample_per_pixel = 10; //count of random sample for each pixel


private:

	void InitData();
	void CleanUp();
	color RaySceneColor(const Ray& r, const Hittable& world) const;

	//@brief Create a ray from the origin directed at a randomly sampled point around the pixel location of x, y
	Ray GetRayAt(int x, int y);

	inline Vec3 sample_pixel_square() const 
	{
		//Returns a random vec3 within the range of -0.5, +0.5
		return Vec3(random_double() - 0.5, random_double() - 0.5, 0);
	}

private:

	int    image_h;   // Rendered image height
	double pixel_color_scale;  // Color scale factor for a sum of pixel samples
	point3 camera_center;         // Camera center
	point3 pixel00_loc;    // Location of pixel 0, 0
	Vec3   pixel_delta_x;  // Offset to pixel to the right
	Vec3   pixel_delta_y;  // Offset to pixel below

	SDL_Window* window;
	SDL_Renderer* render;
	SDL_Texture* render_image;

};