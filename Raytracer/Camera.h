#pragma once
#include "Vector.h"
#include "Ray.h"
#include "Random.h"
#include <stdio.h>
class Camera
{
public:
	Camera(float3 location = float3(0, 0, 0), float3 lookAt = float3(1, 0, 0), float3 vup = float3(0,0,1), float FOV = 75.0f, float aspectRatio = 16.0f/9, float focus_dist = 5.0f, float aperture = 2.0f);
	~Camera();

	Ray GetRay(float u, float v)const;

	Vector3 origin;
	Vector3 ul; 
	Vector3 vertical;
	Vector3 horizontal; 
	float3 front, side, up;
	float near, far;
	float lens_radius;
	static float3 RandomInUnitDisk() {
		float3 p;
		do
		{
			p = float3(0, Random::Next(), Random::Next()) * 2 - float3(0, 1, 1);
		} while (p.lengthSqr() >= 1);
		//printf_s("{%f, %f, %f}", p.x, p.y, p.z);
		return p; 
	}
};

