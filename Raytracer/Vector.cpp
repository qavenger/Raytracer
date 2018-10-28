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
