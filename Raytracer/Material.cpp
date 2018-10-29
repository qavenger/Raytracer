#include "pch.h"
#include "Material.h"

bool Lambertian::Scatter(const Ray & ray, const HitInfo & hit, Vector3 & attenuation, Ray & scatteredRay) const
{
	Vector3 target = hit.HitNormal + RandomInUnitSphere();
	scatteredRay = Ray(hit.HitPoint, target);
	attenuation = albedo;
	return true;
}

bool Metal::Scatter(const Ray & ray, const HitInfo & hit, Vector3 & attenuation, Ray & scatteredRay) const
{
	Vector3 reflected = reflect(ray.Direction(), hit.HitNormal);
	scatteredRay = Ray(hit.HitPoint, reflected + RandomInUnitSphere() * roughness);
	attenuation = albedo;
	return scatteredRay.Direction().dot(hit.HitNormal) > 0;
}

bool Dielectric::Scatter(const Ray & ray, const HitInfo & hit, Vector3 & attenuation, Ray & scatteredRay) const
{
	Vector3 outNormal;
	Vector3 reflected = reflect(ray.Direction(), hit.HitNormal);
	float iot;
	attenuation = Vector3(1, 1, 1);
	float reflect_prob;
	float cosine;
	float theta = ray.Direction().dot(hit.HitNormal);
	if (theta > 0)
	{
		outNormal = -hit.HitNormal;
		iot = ref_idx;
		cosine = ref_idx * theta;
	}
	else
	{
		outNormal = hit.HitNormal;
		iot = 1 / ref_idx;
		cosine = -theta;
	}

	Vector3 refracted;
	if (refract(ray.Direction(), outNormal, iot, refracted))
	{
		reflect_prob = schlick(cosine, ref_idx);
		
	}
	else
	{
		reflect_prob = 1;
	}
	if (drand() < reflect_prob)
	{
		scatteredRay = Ray(hit.HitPoint, reflected);
	}
	else
	{
		scatteredRay = Ray(hit.HitPoint, refracted);
	}
	return true;
}
