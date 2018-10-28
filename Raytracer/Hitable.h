#pragma once
#include "Ray.h"
struct HitInfo
{
	Vector3 HitPoint;
	Vector3 HitNormal;
	float HitTime = -1;
};

class Hitable
{
public:
	virtual bool intersectRay(const Ray& ray, HitInfo* hit)const = 0;
	virtual bool intersectRay(const Ray& ray)const = 0;
};

