#include "pch.h"
#include "Camera.h"


Camera::Camera():
	ul(1, -2, 1),
	vertical(0, 0, -2),
	horizontal(0, 4, 0)
{
}


Camera::~Camera()
{
}

Ray Camera::GetRay(float u, float v) const
{
	return Ray(origin, ul + horizontal * u + vertical * v, 0, 1000);

}
