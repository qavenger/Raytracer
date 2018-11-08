#include "pch.h"
#include "MovingSphere.h"


MovingSphere::MovingSphere()
{
}

MovingSphere::MovingSphere(float3 center, float radius, Material * mat)
	:m_center(center), m_radius(radius)
{
	m_material = mat;
}

void MovingSphere::SetVelocity(float3 vel)
{
	m_velocity = vel;
}


MovingSphere::~MovingSphere()
{
}

float3 MovingSphere::Center(float time) const
{
	return m_center + m_velocity * time;
}

bool MovingSphere::intersectRay(const Ray & ray, HitInfo * hit) const
{
	float3 center = Center(ray.Time());
	Vector3 oc = ray.Origin() - center;
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
		hit->HitNormal = (hit->HitPoint - center) / m_radius;
		hit->HitMat = m_material;
	RS:
		return found;
	}
	return false;
}
