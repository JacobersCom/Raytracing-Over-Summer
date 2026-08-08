#pragma once

#include <cmath>

//Redo whole class to not call functions for every operator, but index into the vector
class Vec3
{
public:

	double vec[3];

	Vec3() : vec{ 0.0, 0.0, 0.0 } {};
	Vec3(double x, double y, double z) : vec{ x, y, z } {};

	double x() const { return vec[0]; };
	double y() const { return vec[1]; };
	double z() const { return vec[2]; };

	Vec3 operator-() const { return Vec3(-vec[0], -vec[1], -vec[2]); };
	double operator[](int i) const { return vec[i]; };
	double operator[](int i) { return vec[i]; };

	Vec3& operator+=(const Vec3& v)
	{
		vec[0] += v.vec[0];
		vec[1] += v.vec[1];
		vec[2] += v.vec[2];
		return *this;
	}

	Vec3& operator*= (double s)
	{
		vec[0] *= s;
		vec[1] *= s;
		vec[2] *= s;

		return *this;
	}

	Vec3& operator/= (double s)
	{
		return *this *= 1 / s;
	}

	double magnitude() const
	{
		return std::sqrt(length_sqrt());
	}

	double length_sqrt() const
	{
		return (vec[0] * vec[0] + vec[1] * vec[1] + vec[2] * vec[2]);
	}

	static Vec3 random()
	{
		return Vec3(random_double(), random_double(), random_double());
	}
	
	static Vec3 random(double min, double max)
	{
		return Vec3(random_double(min, max), random_double(min, max), random_double(min, max));
	}
};

//Just a alias
using point3 = Vec3;

inline Vec3 operator+(const Vec3& v, const Vec3& u)
{
	return Vec3(v.x() + u.x(), v.y() + u.y(), v.z() + u.z());
}


inline Vec3 operator-(const Vec3& v, const Vec3& u)
{
	return Vec3(v.x() - u.x(), v.y() - u.y(), v.z() - u.z());
}


inline Vec3 operator*(const Vec3& v, const Vec3& u)
{
	return Vec3(v.x() * u.x(), v.y() * u.y(), v.z() * u.z());
}

inline Vec3 operator*(double t, const Vec3& v) 
{
	return Vec3(t * v.x(), t * v.y(), t * v.z());
}

inline Vec3 operator*(const Vec3& u, double s)
{
	return s * u;
}

inline Vec3 operator/(const Vec3& u, double s)
{
	return (1/s) * u;
}

inline double dot(const Vec3& v, const Vec3& u)
{
	return v.x() * u.x()
		+ v.y() * u.y()
		+ v.z() * u.z();
}

inline Vec3 cross(const Vec3& v, const Vec3& u)
{
	return Vec3
	(
		(v.y() * u.z() - v.z() * u.y()),
		(v.z() * u.x() - v.x() * u.z()),
		(v.x() * v.y() - v.y() * v.x())
	);
}

inline Vec3 unit_vector(const Vec3& v) {
	return v / v.magnitude();
}

inline Vec3 random_unit()
{
	while (true)
	{
		//Create a random vec between -1 and 1
		Vec3 random_vec = Vec3::random(-1, 1);
		//Extent the vector
		double random_vec_sqrt = random_vec.length_sqrt();
		//If the vector is inside or on the surface of the sphere
		//1e-160 is to ensure the we dont get a bad vector(A vector close to the center of the sphere)
		if (1e-160 < random_vec_sqrt && random_vec_sqrt <= 1.0f)
		{
			//Return a normalized vector
			return random_vec / sqrt(random_vec_sqrt);
		}
	}
}

inline Vec3 random_vec_on_sphere(const Vec3& normal)
{
	Vec3 random_vec = random_unit();
	//If the dot product between the random unit vector and the surface normal
	//is greater than one return the unit vector
	if (dot(random_vec, normal) > 0.0f)
	{
		return random_vec;
	}

	//Else invert the vector to point it in the same direction as the surface normal
	return -random_vec;
}

