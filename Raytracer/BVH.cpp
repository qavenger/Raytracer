#include "pch.h"
#include "BVH.h"
#include "Random.h"

namespace {
	int comp_x(const void* a, const void* b)
	{
		AABB aabb_l, aabb_r;
		Hitable * ah = *(Hitable**)a;
		Hitable * bh = *(Hitable**)b;
		if (!ah->bounding_box(aabb_l) || !bh->bounding_box(aabb_r))
		{
			std::cerr << "No bound";
		}
		if (aabb_l.Min().x - aabb_r.Min().x < 0)
		{
			return -1;
		}
		return 1;
	}

	int comp_y(const void* a, const void* b)
	{
		AABB aabb_l, aabb_r;
		Hitable * ah = *(Hitable**)a;
		Hitable * bh = *(Hitable**)b;
		if (!ah->bounding_box(aabb_l) || !bh->bounding_box(aabb_r))
		{
			std::cerr << "No bound";
		}
		if (aabb_l.Min().y - aabb_r.Min().y < 0)
		{
			return -1;
		}
		return 1;
	}

	int comp_z(const void* a, const void* b)
	{
		AABB aabb_l, aabb_r;
		Hitable * ah = *(Hitable**)a;
		Hitable * bh = *(Hitable**)b;
		if (!ah->bounding_box(aabb_l) || !bh->bounding_box(aabb_r))
		{
			std::cerr << "No bound";
		}
		if (aabb_l.Min().z - aabb_r.Min().z < 0)
		{
			return -1;
		}
		return 1;
	}
}

BVH::BVH()
{
}

BVH::BVH(Hitable ** l, int n)
{
#define DIM 2
	int axis = (int)(DIM * Random::Next());
	if (axis == 0)
	{
		qsort(l, n, sizeof(Hitable*), comp_x);
	}
	else if (axis == 1)
	{
		qsort(l, n, sizeof(Hitable*), comp_y);

	}
	else
	{
		qsort(l, n, sizeof(Hitable*), comp_z);
	}
	if (n == 1)
	{
		left = right = l[0];
	}
	else if (n == 2)
	{
		left = l[0];
		right = l[1];
	}
	else
	{
		left = new BVH(l, n >> 1);
		right = new BVH(l + (n >> 1), n - (n >> 1));
	}
	AABB aabb_l, aabb_r;
	if (!left->bounding_box(aabb_l) || !right->bounding_box(aabb_r))
	{
		std::cerr << "No bounding box is constructed in BVH";
	}
	AABB::SurroundingBox(aabb_l, aabb_r, bound);
}

bool BVH::intersectRay(const Ray & ray, HitInfo * hit) const
{
	if (bound.Hit(ray))
	{
		HitInfo hit_l, hit_r;
		bool isHit_l = left->intersectRay(ray, &hit_l);
		bool isHit_r = right->intersectRay(ray, &hit_r);
		*hit = hit_l;
		if (isHit_l && isHit_r)
		{
			if (hit_r.HitTime < hit_l.HitTime)
			{
				*hit = hit_r;
			}
			return true;
		}
		else if (isHit_l)
		{
			return true;
		}
		else if (isHit_r)
		{
			*hit = hit_r;
			return true;
		}
		return false;
	}
	return false;
}

bool BVH::bounding_box(AABB & aabb) const
{
	aabb = bound;
	return true;
}
