#include "pch.h"
#include "AABB.h"

AABB::AABB(const float3 & min, const float3 & max)
	: m_point{min, max}
{
}


AABB::~AABB()
{
}

bool AABB::Hit(const Ray & r) const
{
	float3 inv_Dir = 1.0f / r.Direction();
	float3 t0 = (m_point[0] - r.Origin()) * inv_Dir;
	float3 t1 = (m_point[1] - r.Origin()) * inv_Dir;
	float tMin = r.TMin();
	float tMax = r.TMax();
	for (int i = 0; i < 3; ++i)
	{
		if (inv_Dir[i] < 0.0f) {
			std::swap(t0[i], t1[i]);
		}
		tMin = t0[i] > tMin ? t0[i] : tMin;
		tMax = t1[i] > tMax ? t1[i] : tMax;
		if (tMax <= tMin) return false;
	}
	
	return true;
}
