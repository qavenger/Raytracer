#pragma once
#include "Hitable.h"
#include <vector>
class HitableList :
	public Hitable
{
public:
	HitableList() {};
	//HitableList(Hitable** list, int size);
	~HitableList();
	void Add(Hitable* hitable);
private:
	std::vector<Hitable*> m_list;
public:
	// Inherited via Hitable
	virtual bool intersectRay(const Ray & ray, HitInfo * hit) const override;

	// Í¨¹ý Hitable ¼Ì³Ð
	virtual bool bounding_box(AABB & aabb) const override;
};

