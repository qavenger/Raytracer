#include "pch.h"
#include "Ray.h"


Ray::Ray() :
	o(), d(1, 0, 0), tMin(0.01f), tMax(1000)
{
}

Ray::Ray(const Vector3 & origin, const Vector3 & direction)
	:o(origin), d(direction.normalized()), tMin(0.01f), tMax(1000)
{
}

Ray::Ray(const Vector3 & origin, const Vector3 & direction, float time)
	: o(origin), d(direction.normalized()), tMin(0.01f), tMax(1000), time(time)
{
}

Ray::Ray(const Vector3 & origin, const Vector3 & direction, float tMin, float tMax)
	: o(origin), d(direction.normalized()), tMin(tMin), tMax(tMax)
{
}

Ray::Ray(const Vector3 & origin, const Vector3 & direction, float tMin, float tMax, float time)
	: o(origin), d(direction.normalized()), tMin(tMin), tMax(tMax), time(time)
{
}
