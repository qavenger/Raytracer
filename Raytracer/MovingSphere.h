#pragma once
#include "Hitable.h"
class MovingSphere : public Hitable
{
public:
	MovingSphere();
	MovingSphere(float3 center, float radius, Material* mat);
	void SetVelocity(float3 vel);
	~MovingSphere();

	float3 Center(float time)const;

	// 通过 Hitable 继承
	virtual bool intersectRay(const Ray & ray, HitInfo * hit) const override;

private:

	float3 m_center;
	float3 m_velocity;
	float m_radius;


	// 通过 Hitable 继承
	virtual bool bounding_box( AABB & aabb) const override;

};

