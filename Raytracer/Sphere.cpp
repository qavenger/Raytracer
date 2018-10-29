#include "pch.h"
#include "Sphere.h"

Vector3 RandomInUnitSphere()
{
	return (Vector3(drand(), drand(), drand()) * 2 - 1).normalized();
}

Sphere::Sphere(const Vector3 & center, float radius):
	m_center(center), m_radius(radius)
{
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
	float b = 2 * oc.dot(ray.Direction());
	float c = distSqr - m_radius * m_radius;
	float disc = b * b - 4 * c;
	if (disc >= 0)
	{
		float discSqrt = sqrtf(disc);
		float r1 = 0.5f * (-b + discSqrt);
		float r2 = 0.5f * (-b - discSqrt);
		hit->HitTime = (r1 * r2) < 0 ? fmax(r1, r2) : (r1 < 0 ? -1 : fmin(r1, r2));
		hit->HitPoint = ray.Origin() + ray.Direction() * hit->HitTime;
		hit->HitNormal = (hit->HitPoint - m_center)/m_radius;
		hit->HitMat = m_material;
	}
	return hit->HitTime >= ray.TMin() && hit->HitTime<=ray.TMax();
}

bool Sphere::intersectRay(const Ray & ray) const
{
	Vector3 oc = ray.Origin() - m_center;
	float b = 2 * oc.dot(ray.Direction());
	float c = oc.lengthSqr() - m_radius * m_radius;
	float disc = b * b - 4 * c;
	return disc>=0;
}


/*

	(p - c)^2 = r ^ 2
	(o - c + td) ^ 2 = r ^ 2

	(o-c)(o-c) - r*r + 2t(o-c)d + t*t;

	4(o-c)
*/