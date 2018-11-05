#pragma once
#include "Vector.h"
#include "Ray.h"
class Camera
{
public:
	Camera(float3 location = float3(0, 0, 0), float3 lookAt = float3(1, 0, 0), float3 vup = float3(0,0,1), float FOV = 75.0f, float aspectRatio = 16.0f/9, float near = 0.1f, float far = 1000.0f);
	~Camera();

	Ray GetRay(float u, float v)const;

	Vector3 origin;
	Vector3 ul; 
	Vector3 vertical;
	Vector3 horizontal; 
	float near, far;
};

