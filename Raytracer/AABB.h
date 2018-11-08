#pragma once
#include "Ray.h"
//#include "Vector.h"
class AABB
{
public:
	AABB()= default;
	AABB(const float3& min, const float3& max);
	~AABB();
	float3 Min()const { return m_point[0]; }
	float3 Max()const { return m_point[1]; }
	float3 operator[](int i)const { assert(i == 0 || i == 1); return m_point[i]; }
	float3& Min() { return m_point[0]; }
	float3& Max() { return m_point[1]; }
	float3& operator[](int i){ assert(i == 0 || i == 1); return m_point[i]; }
	float3 Corner(int i) { 
		assert(i >= 0 && i < 8); 
		return float3(
			m_point[ (i >> 0) & 1 ].x,
			m_point[ (i >> 1) & 1 ].y,
			m_point[ (i >> 2) & 1 ].z
		);
	}
	bool Hit(const Ray& r)const;
private:
	float3 m_point[2];
};

