#pragma once
#include "Ray.h"
#include "Hitable.h"
class Material
{
public:
	virtual bool Scatter(const Ray& ray, const HitInfo& hit, Vector3& attenuation, Ray& scatteredRay)const = 0;
};


class Lambertian : public Material
{
public:
	Lambertian(const Vector3 albedo) : albedo(albedo){}

	Vector3 albedo;

	// Inherited via Material
	virtual bool Scatter(const Ray & ray, const HitInfo & hit, Vector3 & attenuation, Ray & scatteredRay) const override;
};

class Metal : public Material
{
public:
	Metal(const Vector3& albedo, float roughness) :albedo(albedo), roughness(saturate(roughness)) {}

	Vector3 albedo;
	float roughness;
	// Inherited via Material
	virtual bool Scatter(const Ray & ray, const HitInfo & hit, Vector3 & attenuation, Ray & scatteredRay) const override;
};

class Dielectric : public Material
{
public:
	Dielectric(float idx) :ref_idx(idx) {}

	float ref_idx;

	// Inherited via Material
	virtual bool Scatter(const Ray & ray, const HitInfo & hit, Vector3 & attenuation, Ray & scatteredRay) const override;
};