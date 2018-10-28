#include "pch.h"
#include "Ray.h"


Ray::Ray() :
	o(), d(1, 0, 0)
{
}

Ray::Ray(const Vector3 & origin, const Vector3 & direction)
	:o(origin), d(direction.normalized()), tMin(0.01f), tMax(1000)
{
}

Ray::Ray(const Vector3 & origin, const Vector3 & direction, float tMin, float tMax)
	: o(origin), d(direction.normalized()), tMin(tMin), tMax(tMax)
{
}
