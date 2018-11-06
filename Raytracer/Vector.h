#pragma once
#include "pch.h"

typedef class Vector3 : public DirectX::XMFLOAT3
{
public:

#define OPERATOR_U(op) Vector3& operator op (const Vector3& v){ x op v.x; y op v.y; z op v.z; return *this; }
#define OPERATOR_B(op) Vector3 operator op (const Vector3& v)const{ return Vector3(x op v.x, y op v.y, z op v.z); }

#define OPERATOR_US(op) Vector3& operator op (float s){ x op s; y op s; z op s; return *this; }
#define OPERATOR_BS(op) Vector3 operator op (float s)const{ return Vector3(x op s, y op s, z op s); }


	Vector3();
	Vector3(float x, float y, float z);
	Vector3(float s);
	Vector3(_In_reads_(3) const float *pArray);
	Vector3(const Vector3&) = default;
	Vector3& operator=(const Vector3&) = default;
	Vector3(Vector3&&) = default;
	Vector3& operator=(Vector3&&) = default;

	OPERATOR_U(+=)
	OPERATOR_U(-=)
	OPERATOR_U(*=)
	OPERATOR_U(/=)
	OPERATOR_US(+=)
	OPERATOR_US(-=)
	OPERATOR_US(*=)
	Vector3& operator/= (float s) { float sinv = 1 / s; x *= sinv; y *= sinv; z *= sinv; return *this; }
	OPERATOR_B(+)
	OPERATOR_B(-)
	OPERATOR_B(*)
	OPERATOR_B(/)
	OPERATOR_BS(+)
	OPERATOR_BS(-)
	OPERATOR_BS(*)
	Vector3 operator/(float s)const { float s_inv = 1 / s; return Vector3(x*s_inv, y*s_inv, z*s_inv); }

	Vector3 operator+()const { return *this; }
	Vector3 operator-()const { return Vector3(-x, -y, -z); }

#define FRIEND_OPERATOR(op) friend Vector3 operator op(float s, const Vector3& v){return Vector3(s op v.x, s op v.y, s op v.z);}
	FRIEND_OPERATOR(+)
	FRIEND_OPERATOR(-)
	FRIEND_OPERATOR(*)
	FRIEND_OPERATOR(/)

	inline float length()const { return sqrt(x*x + y * y + z * z); }
	inline float lengthSqr()const { return x * x + y * y + z * z; }

	float operator[](int i)const { assert(i >= 0 && i < 3); return i == 0 ? x : i == 1 ? y : z; }
	float& operator[](int i) { assert(i >= 0 && i < 3); return i == 0 ? x : i == 1 ? y : z; }

	Vector3 normalized()const { 
		float len_inv = 1 / sqrt(x*x + y * y + z * z); 
		assert(isfinite(len_inv)); 
		return Vector3(x*len_inv, y*len_inv, z*len_inv); 
	}

	Vector3& normalize() {
		float len_inv = 1 / sqrt(x*x + y * y + z * z);
		assert(isfinite(len_inv));
		x *= len_inv;
		y *= len_inv;
		z *= len_inv;
		return *this;
	}

	friend float dot(const Vector3& v1, const Vector3& v2) { return v1.x*v2.x + v1.y*v2.y + v1.z*v2.z; }
	friend Vector3 cross(const Vector3& v1, const Vector3& v2) { return Vector3( v1.y*v2.z - v1.z*v2.y, v1.z*v2.x - v1.x*v2.z, v1.x*v2.y - v1.y*v2.x ); }
	friend Vector3 min(const Vector3& v1, const Vector3& v2) {
		return Vector3(std::fminf(v1.x, v2.x), std::fminf(v1.y, v2.y), std::fminf(v1.z, v2.z));
	}
	friend Vector3 max(const Vector3& v1, const Vector3& v2) {
		return Vector3(std::fmaxf(v1.x, v2.x), std::fmaxf(v1.y, v2.y), std::fmaxf(v1.z, v2.z));
	}
	friend Vector3 saturate(const Vector3& v) { return max(min(v, Vector3(1,1,1)), Vector3(0,0,0)); }
	friend std::ostream& operator<<(std::ostream& os, const Vector3& v) {
		os << '{' << v.x << ", " << v.y << ", " << v.z << '}';
		return os;
	}
	friend Vector3 normalize(const Vector3& v) { 
		float len_inv = 1 / sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
		assert(isfinite(len_inv));
		return Vector3(v.x * len_inv, v.y * len_inv, v.z * len_inv);
	}
	friend Vector3 reflect(const Vector3& v, const Vector3& n) {
		return v - 2 * n * dot(v, n);
	}
	friend float dist(const Vector3& v0, const Vector3& v1) { return (v1 - v0).length(); }
	friend float length(const Vector3& v) { return v.length(); }
	friend bool refract(const Vector3& v, const Vector3& n, float IOR, Vector3& refracted)
	{
		float von = dot(v, n);
		float discr = 1 - IOR * IOR * (1 - von * von);
		if (discr > 0)
		{
			refracted = (v - n * von)*IOR - n * sqrtf(discr);
			return true;
		}
		return false;
	}
	bool isnan()const { return std::isnan(x) || std::isnan(y) || std::isnan(z); }
	
} float3;



float saturate(float val);
float schlick(float cosine, float index);

Vector3 RandomInUnitSphere();

template<typename T>
T lerp(T a, T b, float t) {
	return a + (b - a)*t;
}
