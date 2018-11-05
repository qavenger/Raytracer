#include "pch.h"
#include "Camera.h"


Camera::Camera(float3 location, float3 lookAt, float3 vup, float FOV, float aspectRatio, float near, float far):
	origin(location), near(near), far(far)
{
	float3 front = normalize(lookAt - location);
	float3 side = normalize(cross(vup, front));
	float3 up = cross(front, side);
	float theta = FOV * DEG2RAD;
	float halfHeight = tan(theta*0.5f) * near;
	float halfWidth = aspectRatio * halfHeight;
	//ul = Vector3(1, -halfWidth , halfHeight);
	ul = origin + front * near - side * halfWidth + up * halfHeight;
	vertical = -2 * halfHeight * up;
	horizontal = 2 * halfWidth * side;
}


Camera::~Camera()
{
}

Ray Camera::GetRay(float u, float v) const
{
	return Ray(origin, (ul + horizontal * u + vertical * v) - origin, near, far);

}
