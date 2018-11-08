#include "pch.h"
#include "Camera.h"

Camera::Camera(float3 location, float3 lookAt, float3 vup, float FOV, float aspectRatio, float focus_dist, float aperture, float openDuration) :
	origin(location), lens_radius(aperture * 0.5f), openDuration(openDuration)
{
	float theta = FOV * DEG2RAD;
	float halfHeight = tan(theta*0.5f) * focus_dist;
	float halfWidth = aspectRatio * halfHeight;
	front = normalize(lookAt - location);
	side = normalize(cross(vup, front));
	up = cross(front, side);
	//ul = Vector3(1, -halfWidth , halfHeight);
	ul = origin + front * focus_dist - side * halfWidth + up * halfHeight;
	vertical = -2 * halfHeight * up;
	horizontal = 2 * halfWidth * side;
}


Camera::~Camera()
{
}

Ray Camera::GetRay(float u, float v) const
{
	float3 rd = lens_radius * RandomInUnitDisk();
	//std::cout << rd << std::endl; 
	float3 offset = side * rd.y + up * rd.z;


	/*printf("{%f, %f, %f}\n", rd.x, rd.y, rd.z);

	printf("{%f, %f, %f}\n", offset.x, offset.y, offset.z);*/
	float t = Random::Next() * openDuration;
	//if (!isfinite(t)) t = 0;
	float3 rOrigin = origin + offset;
	return Ray(rOrigin, ul + horizontal * u + vertical * v - rOrigin, t);
}
