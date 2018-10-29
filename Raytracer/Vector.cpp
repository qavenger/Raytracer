#include "pch.h"
#include "Vector.h"


Vector3::Vector3()
	:v{0,0,0}
{
}

Vector3::Vector3(float x, float y, float z)
	:v{x,y,z}
{
}

Vector3::Vector3(float s)
	:v{s,s,s}
{
}

Vector3::Vector3(float in[3])
	: v{in[0], in[1], in[2]}
{
}

Vector3::Vector3(const Vector3 & v)
	:v{v.v[0], v.v[1], v.v[2]}
{
}

Vector3 & Vector3::operator=(const Vector3 & other)
{
	v[0] = other.v[0];
	v[1] = other.v[1];
	v[2] = other.v[2];
	return *this;
}

bool Vector3::operator==(const Vector3 & other) const
{
	return v[0]==other.v[0] && v[1] == other.v[1] && v[2] == other.v[2];
}

float drand()
{
	return (float)(rand() * inv_randmax);
}

Vector3 reflect(const Vector3 & v, const Vector3 & n)
{
	return v - n * 2 * v.dot(n);
}

bool refract(const Vector3 & v, const Vector3 & n, float Index, Vector3 & refracted)
{
	float von = v.dot(n);
	float discr = 1 - Index * Index * (1-von*von);
	if (discr > 0)
	{
		refracted = (v - n * von)*Index - n * sqrtf(discr);
		return true;
	}
	return false;
}


Vector3 saturate(Vector3 val)
{
	for (int i = 0; i < 3; ++i)
	{
		val[i] = fminf(fmaxf(val[i], 0), 1);
	}
	return val;
}

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
