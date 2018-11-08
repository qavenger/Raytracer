#pragma once
#include "Vector.h"
class Ray
{
public:
	Ray();
	Ray(const Vector3& origin, const Vector3& direction);
	Ray(const Vector3& origin, const Vector3& direction, float time);
	Ray(const Vector3& origin, const Vector3& direction, float tMin, float tMax);
	Ray(const Vector3& origin, const Vector3& direction, float tMin, float tMax, float time);
	inline Vector3 GetPoint(float t)const { return o + d * t; }
	inline Vector3 Origin()const { return o; }
	inline Vector3 Direction()const { return d; }
	inline float TMin()const { return tMin; }
	inline float TMax()const { return tMax; }
	float& TMax() { return tMax; }
	float& TMin() { return tMin; }
	float Time()const { return time; }
private:
	Vector3 o, d;
	float tMin, tMax;
	float time;
};

