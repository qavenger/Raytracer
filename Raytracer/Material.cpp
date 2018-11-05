#include "pch.h"
#include "Material.h"
#include "Random.h"

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
	return dot(scatteredRay.Direction(), hit.HitNormal) > 0;
}

bool Dielectric::Scatter(const Ray & ray, const HitInfo & hit, Vector3 & attenuation, Ray & scatteredRay) const
{
	Vector3 outNormal;
	Vector3 reflected = reflect(ray.Direction(), hit.HitNormal);
	float ior;
	attenuation = 1.0f;
	float cosine;
	float theta = dot(ray.Direction(), hit.HitNormal);
	if (theta > 0)
	{
		outNormal = -hit.HitNormal;
		ior = ref_idx;
		cosine = ref_idx * theta;
	}
	else
	{
		outNormal = hit.HitNormal;
		ior = 1 / ref_idx;
		cosine = -theta;
	}

	Vector3 refracted;
	if (refract(ray.Direction(), outNormal, ior, refracted))
	{
		float reflect_prob = schlick(cosine, ref_idx);
		if (Random::Next() >= reflect_prob)
		{
			scatteredRay = Ray(hit.HitPoint, refracted);
		}
	}
	else
	{
		scatteredRay = Ray(hit.HitPoint, reflected);
	}
	
	return true;
}
