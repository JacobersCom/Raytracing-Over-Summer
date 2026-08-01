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


private:

	void InitData();
	void CleanUp();
	color RaySceneColor(const Ray& r, const Hittable& world) const;

private:

	int    image_h;   // Rendered image height
	
	point3 camera_center;         // Camera center
	point3 upper_left_pixel_loc;    // Location of pixel 0, 0
	Vec3   pixel_delta_x;  // Offset to pixel to the right
	Vec3   pixel_delta_y;  // Offset to pixel below

	SDL_Window* window;
	SDL_Renderer* render;
	SDL_Texture* render_image;

};