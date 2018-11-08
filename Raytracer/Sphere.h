#pragma once
#include "Hitable.h"
#include "Vector.h"
class Sphere : public Hitable
{
public:
	Sphere(){};
	Sphere(const Vector3& center, float radius, Material* mat = nullptr);
	virtual ~Sphere();
	virtual bool intersectRay(const Ray& ray, HitInfo* hit)const override;
private:
	float m_radius = 0;
	Vector3 m_center;

	// Í¨¹ý Hitable ¼Ì³Ð
	virtual bool bounding_box(float t0, float t1, AABB & aabb) const override;
};

