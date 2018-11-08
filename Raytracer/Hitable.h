#pragma once
#include "Ray.h"
#include "AABB.h"
class Material;
struct HitInfo
{
	Vector3 HitPoint;
	Vector3 HitNormal;
	float HitTime = -1;
	Material* HitMat;
};

class Hitable
{
public:
	virtual bool intersectRay(const Ray& ray, HitInfo* hit)const = 0;
	virtual bool bounding_box(float t0, float t1, AABB& aabb)const = 0;
public:
	Material* m_material;
};

