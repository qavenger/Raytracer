#include "pch.h"
#include "Vector.h"
#include "Random.h"

float saturate(float val)
{
	return fminf(fmaxf(val, 0), 1);
}

float schlick(float cosine, float index)
{
	float r0 = (1 - index) / (1 + index);
	r0 = r0 * r0;
	cosine = 1 - cosine;
	float cosSqr = cosine * cosine;
	return r0 + (1 - r0) * cosSqr * cosSqr * cosine;
}

Vector3::Vector3() :
	DirectX::XMFLOAT3(0, 0, 0)
{
}

Vector3::Vector3(float x, float y, float z) :
	DirectX::XMFLOAT3(x,y,z)
{
}

Vector3::Vector3(float s) :
	DirectX::XMFLOAT3(s,s,s)
{
}

Vector3::Vector3(const float* pArray) :
	DirectX::XMFLOAT3(pArray)
{
}


Vector3 RandomInUnitSphere()
{
	
	Vector3 v = Vector3(Random::Next(), Random::Next(), Random::Next()) * 2 - 1;
	
	while (v.lengthSqr() >= 1)
	{
		v = Vector3(Random::Next(), Random::Next(), Random::Next()) * 2 - 1;
	}

	return v;
}