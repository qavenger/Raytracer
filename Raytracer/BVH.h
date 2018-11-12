#pragma once
#include "Hitable.h"
#include "AABB.h"
class BVH : public Hitable
{
public:
	BVH();
	BVH(Hitable** l, int n);
private:
	AABB bound;
	Hitable *left, *right;

	// Í¨¹ý Hitable ¼Ì³Ð
	virtual bool intersectRay(const Ray & ray, HitInfo * hit) const override;
	virtual bool bounding_box(AABB & aabb) const override;
};

