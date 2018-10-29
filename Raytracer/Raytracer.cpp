// Raytracer.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include "Film.h"
#include "Ray.h"
#include "HitableList.h"
#include "Sphere.h"
#include "Camera.h"
#include "Material.h"


Vector3 trace(const Ray& ray, const HitableList* list, int depth)
{
	HitInfo hit;
	if (list->intersectRay(ray, &hit))
	{
		Vector3 attenuation;
		Ray scatteredRay;
		if (depth < 50, hit.HitMat->Scatter(ray, hit, attenuation, scatteredRay))
		{
			return trace(scatteredRay, list, depth + 1) * attenuation;
		}
		return Vector3();
		/*
		Vector3 l = (lightPos - hit.HitPoint).normalized();
		Vector3 h = (view + l).normalized();
		float nol = l.dot(hit.HitNormal);
		float nov = view.dot(hit.HitNormal);
		float noh = h.dot(hit.HitNormal);
		float a = roughness * roughness;
		float a2 = a * a;

		float den = (noh*noh * (a2 - 1) + 1);
		den *= den * 3.14159f;

		float D = a2 / den;

		Vector3 fresnel = SpecularColor + (Vector3(1, 1, 1) - SpecularColor) * powf(1 - l.dot(h), 5);

		/*float Gden = (l.dot(h));
		Gden = 1 / (Gden*Gden);

		float Gden = fminf(fminf((2 * noh*nov / view.dot(h)), 2 * noh*nol / view.dot(h)), 1);

		return saturate(fresnel * D * 0.25f * Gden + saturate(Albedo / 3.141593f * nol));
		*/
	}
	else
	{
		float t = 0.5f * (ray.Direction().z() + 1);
		return lerp(Vector3(1), Vector3(0.5f, 0.7f, 1.0f), t);;
	}
}

Camera cam;
void Color(Film* film, const HitableList* list)
{
	float inv_w = 1.0f / film->GetWidth();
	float inv_h = 1.0f / film->GetHeight();
	Vector3 origin;

	Vector3 lightPos(0, 1, 2);
	Vector3 view(-1, 0, 0);

	Vector3 SpecularColor(1.0f, 0.76f, 0.33f);
	//Vector3 Albedo(0.5f, 0.615f, 0.842f);
	Vector3 Albedo(0.04f);
	float roughness = 0.524f;
	int32 sampleCount = 100;
	int32 totalPix = film->GetHeight() * film->GetWidth();
	int32 percentage = 0;
	for (uint32 y = 0; y < film->GetHeight(); ++y)
	{
		for (uint32 x = 0; x < film->GetWidth(); ++x)
		{
			uint32 index = y * film->GetWidth() + x;
			
			Vector3 c;
			for (int s = 0; s < sampleCount; ++s)
			{
				float u = (x + drand())* inv_w;
				float v = (y + drand())* inv_h;
				Ray ray = cam.GetRay(u, v);
				c += trace(ray, list, 0);
			}
			film->GetFilmData()[index] = c / (float)sampleCount;
			int32 cur = (int32)(((float)index / totalPix) * 100);
			if (cur > percentage)
			{
				percentage = cur;
				std::cout << percentage << '\n';
			}
		}
	}
}


#define NUM_OF_OBJ 5
#include <time.h>
int main()
{
	srand((unsigned int)time(0));
	Film film(455, 256);
	Hitable* spheres[NUM_OF_OBJ];

	Lambertian lam(Vector3(0.38f, 0.42f, 0.5f));
	Metal metal0(Vector3(0.8f), 0.5f);
	Metal metal1(Vector3(0.8f, 0.6f, 0.2f), 0.1f);
	Dielectric diel(1.7f);
	spheres[0] = new Sphere(Vector3(6, 0, 0), 2);
	spheres[0]->m_material = &lam;
	spheres[1] = new Sphere(Vector3(6, 0, -102), 100);
	spheres[1]->m_material = &lam;
	spheres[2] = new Sphere(Vector3(5.5f, 4, 0), 2);
	spheres[2]->m_material = &metal0;
	spheres[3] = new Sphere(Vector3(5.5f, -4, 0), 2);
	spheres[3]->m_material = &diel;
	spheres[4] = new Sphere(Vector3(5.5f, -4, 0), -1.9f);
	spheres[4]->m_material = &diel;
	//for (int i = 1; i < 10; ++i)
	//{
	//	spheres[i] = new Sphere(Vector3((float)rand() / RAND_MAX * 10 + 3, (float)rand() / RAND_MAX * 60 - 30, (float)rand() / RAND_MAX * 20 - 10), (float)rand() / RAND_MAX * 4 + 1);
	//	//spheres[i] = new Sphere(Vector3(3, (i - 4), 0), (float)rand() / RAND_MAX * 2);
	//	//spheres[i] = new Sphere();
	//}

	HitableList list(spheres, NUM_OF_OBJ);

	Color(&film, &list);
	film.ToPPM();
	for (int i = 0; i < NUM_OF_OBJ; ++i)
	{
		delete spheres[i];
	}
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
