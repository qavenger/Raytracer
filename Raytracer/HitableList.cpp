#include "pch.h"
#include "HitableList.h"



HitableList::HitableList(Hitable ** list, int size)
{
	for (int i = 0; i < size; ++i)
	{
		m_list.push_back(list[i]);
	}
}

HitableList::~HitableList()
{
}

bool HitableList::intersectRay(const Ray & ray, HitInfo * hit) const
{
	HitInfo info;
	Ray tmpRay = ray;
	bool anyHit = false;
	for (auto obj : m_list)
	{
		if (obj->intersectRay(tmpRay, &info) && info.HitTime < tmpRay.TMax())
		{
			anyHit = true;
			tmpRay.TMax() = info.HitTime;
			*hit = info;
		}
	}
	return anyHit;
}
