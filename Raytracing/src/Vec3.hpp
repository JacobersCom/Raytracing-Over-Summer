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