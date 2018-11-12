#include "pch.h"
#include "HitableList.h"
#include "Material.h"


//
//HitableList::HitableList(Hitable ** list, int size)
//{
//	for (int i = 0; i < size; ++i)
//	{
//		m_list.push_back(list[i]);
//	}
//}

HitableList::~HitableList()
{
	int i = 0;
	for (; i < 5; ++i)
	{
		delete m_list[i];
	}
	for(;i < m_list.size(); ++i)
	{
		delete m_list[i]->m_material;
		delete  m_list[i];
	}
	m_list.clear();
}

void HitableList::Add(Hitable * hitable)
{
	m_list.push_back(hitable);
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

bool HitableList::bounding_box(AABB & aabb) const
{
	if (m_list.size() < 1)return false;
	AABB tmpBox;
	bool first = m_list[0]->bounding_box(tmpBox);
	if (!first) return false;
	else aabb = tmpBox;
	for (int i = 0; i < (int)m_list.size(); ++i)
	{
		if (m_list[i]->bounding_box(tmpBox))
		{
			AABB::SurroundingBox(aabb, tmpBox, aabb);
		}
		else return false;
	}
	return true;
}
