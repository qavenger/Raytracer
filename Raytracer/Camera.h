#pragma once
#include "Vector.h"
#include "Ray.h"
class Camera
{
public:
	Camera();
	~Camera();

	Ray GetRay(float u, float v)const;

	Vector3 origin;
	Vector3 ul; 
	Vector3 vertical;
	Vector3 horizontal; 
};

