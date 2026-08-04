#pragma once
#include <Interval.hpp>
#include <Vec3.hpp>

using color = Vec3;


/*
* @brief Writes RGB values to a 3 diminaion vector and uses 
  @brief bitwise to bring the values into RGB color format
  @brief returning the value as a uint32_t

  @param Vec3 type
*/
inline uint32_t WriteColor(const color& pixel_color) 
{

	auto r = pixel_color.x();
	auto g = pixel_color.y();
	auto b = pixel_color.z();

	//Translate RGB values from [0-1] to [0-255]
	static const Interval color_intensity(0.000, 0.999);
	auto ir = int(256 * color_intensity.clamp(r));
	auto ig = int(256 * color_intensity.clamp(g));
	auto ib = int(256 * color_intensity.clamp(b));

	
	return ((ir << 24) | (ig << 16) | (ib << 8) | 0xFF);
}