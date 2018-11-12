#include "pch.h"
#include "AABB.h"

AABB::AABB(const float3 & min, const float3 & max)
	: m_point{min, max}
{
}


AABB::~AABB()
{
}

namespace {
	float ffmin(float a, float b) { return a < b ? a : b; }
	float ffmax(float a, float b) { return a > b ? a : b; }
}

bool AABB::Hit(const Ray & r) const
{
	/*float3 inv_Dir = 1.0f / r.Direction();
	float3 t0 = (m_point[0] - r.Origin()) * inv_Dir;
	float3 t1 = (m_point[1] - r.Origin()) * inv_Dir;*/

	float tMin = r.TMin();
	float tMax = r.TMax();
	for (int i = 0; i < 3; ++i)
	{
		float inv_Dir = 1 / r.Direction()[i];
		float t0 = (m_point[0][i] - r.Origin()[i]) * inv_Dir;
		float t1 = (m_point[1][i] - r.Origin()[i]) * inv_Dir;
		if (inv_Dir < 0.0f) {
			std::swap(t0, t1);
		}
		tMin = t0 > tMin ? t0 : tMin;
		tMax = t1 > tMax ? t1 : tMax;
		if (tMax <= tMin) return false;
	}
	
	return true;
}
