#pragma once
#include "pch.h"



class Vector3
{
public:
	Vector3();
	Vector3(float x, float y, float z);
	Vector3(float s);
	Vector3(float in[3]);
	Vector3(const Vector3& v);

	Vector3& operator=(const Vector3& other);
	bool operator==(const Vector3& other)const;

	inline Vector3& operator+=(const Vector3& other) { v[0] += other.v[0]; v[1] += other.v[1]; v[2] += other.v[2]; return *this; }
	inline Vector3& operator-=(const Vector3& other) { v[0] -= other.v[0]; v[1] -= other.v[1]; v[2] -= other.v[2]; return *this; }
	inline Vector3& operator*=(const Vector3& other) { v[0] *= other.v[0]; v[1] *= other.v[1]; v[2] *= other.v[2]; return *this; }
	inline Vector3& operator/=(const Vector3& other) { v[0] /= other.v[0]; v[1] /= other.v[1]; v[2] /= other.v[2]; return *this; }
	inline Vector3& operator*=(const float s) { v[0] *= s; v[1] *= s; v[2] *= s; return *this; }
	inline Vector3& operator/=(const float s) { float inv_s = 1.0f / s; v[0] *= inv_s; v[1] *= inv_s; v[2] *= inv_s; return *this; }

	inline Vector3 operator+(const Vector3& other)const { return Vector3(v[0] + other.v[0], v[1] + other.v[1], v[2] + other.v[2]); }
	inline Vector3 operator-(const Vector3& other)const { return Vector3(v[0] - other.v[0], v[1] - other.v[1], v[2] - other.v[2]); }
	inline Vector3 operator*(const Vector3& other)const { return Vector3(v[0] * other.v[0], v[1] * other.v[1], v[2] * other.v[2]); }
	inline Vector3 operator/(const Vector3& other)const { return Vector3(v[0] / other.v[0], v[1] / other.v[1], v[2] / other.v[2]); }
	inline Vector3 operator*(const float s)const { return Vector3(v[0] * s, v[1] * s, v[2] * s); }
	inline Vector3 operator/(const float s)const { float inv_s = 1.0f / s; return Vector3(v[0] * inv_s, v[1] * inv_s, v[2] * inv_s); }

	inline float dot(const Vector3& other)const { return x() * other.x() + y() * other.y() + z() * other.z(); }
	inline Vector3 cross(const Vector3& other)const { 
		return Vector3( 
			y() * other[2] - z() * other[1], 
			z() * other[0] - x() * other[2], 
			x() * other[1] - y() * other[0]); }

	friend std::istream& operator>>(std::istream& is, Vector3& v) { is >> v.x() >> v.y() >> v.z();  return is; }
	friend std::ostream& operator<<(std::ostream& os, Vector3& v) { os << v.x() << ' ' << v.y() << ' ' << v.z(); return os; }
public:
	inline float x()const { return v[0]; };
	inline float r()const { return v[0]; };
	inline float y()const { return v[1]; };
	inline float g()const { return v[1]; };
	inline float z()const { return v[2]; };
	inline float b()const { return v[2]; };

	inline float& x() { return v[0]; };
	inline float& r() { return v[0]; };
	inline float& y() { return v[1]; };
	inline float& g() { return v[1]; };
	inline float& z() { return v[2]; };
	inline float& b() { return v[2]; };

	inline float operator[](uint32 index)const { assert(index >= 0 && index < 3); return v[index]; }
	inline float& operator[](uint32 index) { assert(index >= 0 && index < 3); return v[index]; }
	inline Vector3 operator-()const { return Vector3(-v[0], -v[1], -v[2]); }

	inline float length()const { return sqrtf(v[0] * v[0] + v[1] * v[1] + v[2] * v[2]); }
	inline float lengthSqr()const { return v[0] * v[0] + v[1] * v[1] + v[2] * v[2]; }
	inline Vector3 normalized()const { float l = 1.0f / this->length(); return Vector3(v[0] * l, v[1] * l, v[2] * l); }
public:
	float v[3];
};



float saturate(float val);
Vector3 saturate(Vector3 val);
float schlick(float cosine, float index);


Vector3 reflect(const Vector3& v, const Vector3& n);
bool refract(const Vector3& v, const Vector3& n, float Index, Vector3& refracted);

Vector3 RandomInUnitSphere();

const double inv_randmax = 1.0f / RAND_MAX;
float drand();

template<typename T>
T lerp(T a, T b, float t) {
	return a + (b - a)*t;
}
