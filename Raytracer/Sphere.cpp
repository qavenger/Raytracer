#include "pch.h"
#include "Sphere.h"


Sphere::Sphere(const Vector3 & center, float radius, Material* mat):
	m_center(center), m_radius(radius)
{
	m_material = mat;
}

Sphere::~Sphere()
{
}

bool Sphere::intersectRay(const Ray & ray, HitInfo* hit) const
{
	Vector3 oc = ray.Origin() - m_center;
	float distSqr = oc.lengthSqr();
	if (distSqr > (ray.TMax() + std::fabsf(m_radius))*(ray.TMax() + std::fabsf(m_radius)))
	{
		return false;
	}
	float b = dot(oc, ray.Direction());
	float c = distSqr - m_radius * m_radius;
	float disc = b * b - c;
	if (disc > 0)
	{
		float discSqrt = sqrtf(disc);
	/*	float r1 = (-b + discSqrt);
		float r2 = (-b - discSqrt);
	*/
		bool found = false;
		float t = -b - discSqrt;

		if (t > ray.TMin() && t < ray.TMax())
		{
			hit->HitTime = t;
			found = true;
			goto FOUND;
		}
		
		t = -b + discSqrt;
		if (t > ray.TMin() && t < ray.TMax())
		{
			hit->HitTime = t;
			found = true;
			goto FOUND;
		}
		goto RS;
		//hit->HitTime = (r1 * r2) < 0 ? fmax(r1, r2) : (r1 < 0 ? -1 : fmin(r1, r2));
FOUND:
		//std::cout << t << std::endl;
		hit->HitPoint = ray.GetPoint(hit->HitTime);
		hit->HitNormal = (hit->HitPoint - m_center)/m_radius;
		hit->HitMat = m_material;
RS:
		return found;
	}
	return false;
}

bool Sphere::bounding_box(float t0, float t1, AABB & aabb) const
{
	aabb = AABB(m_center - m_radius, m_center + m_radius);
	return true;
}



/*

	(p - c)^2 = r ^ 2
	(o - c + td) ^ 2 = r ^ 2

	(o-c)(o-c) - r*r + 2(o-c)dt + t*t;
	a = 1
	b = 2(o-c)d
	c = distSqr - r*r

*/