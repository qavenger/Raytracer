#pragma once
#include "Hitable.h"
#include <vector>
class HitableList :
	public Hitable
{
public:
	HitableList(Hitable** list, int size);
	~HitableList();
private:
	std::vector<Hitable*> m_list;
public:
	// Inherited via Hitable
	virtual bool intersectRay(const Ray & ray, HitInfo * hit) const override;
	virtual bool intersectRay(const Ray & ray) const override;
};

