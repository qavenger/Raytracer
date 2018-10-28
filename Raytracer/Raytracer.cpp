// Raytracer.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include "Film.h"
#include "Vector.h"
#include "Ray.h"
#include "HitableList.h"
#include "Sphere.h"
#include "Camera.h"

template<typename T>
T lerp(T a, T b, float t) {
	return a + (b - a)*t;
}

float saturate(float val)
{
	return fminf(fmaxf(val, 0), 1);
}

Vector3 saturate(Vector3 val)
{
	for (int i = 0; i < 3; ++i)
	{
		val[i] = fminf(fmaxf(val[i], 0), 1);
	}
	return val;
}
const double inv_randmax = 1.0f / RAND_MAX;
float drand()
{
	return (float)(rand() * inv_randmax);
}

Vector3 RandomInUnitSphere()
{
	return (Vector3(drand(), drand(), drand()) * 2 - 1).normalized();
}

Vector3 trace(const Ray& ray, const HitableList* list)
{
	HitInfo hit;
	if (list->intersectRay(ray, &hit))
	{
		Vector3 target = hit.HitPoint + hit.HitNormal + RandomInUnitSphere();
		return trace(Ray(hit.HitPoint, target), list) * 0.5f;
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
				c += trace(ray, list);
			}
			film->GetFilmData()[index] = c / sampleCount;
		}
	}
}



#include <time.h>

int main()
{
	srand(time(0));
	Film film(455, 256);
	Hitable* spheres[2];
	spheres[0] = new Sphere(Vector3(3, 0, 0), 2);
	spheres[1] = new Sphere(Vector3(3, 0, -102), 100);
	//for (int i = 1; i < 10; ++i)
	//{
	//	spheres[i] = new Sphere(Vector3((float)rand() / RAND_MAX * 10 + 3, (float)rand() / RAND_MAX * 60 - 30, (float)rand() / RAND_MAX * 20 - 10), (float)rand() / RAND_MAX * 4 + 1);
	//	//spheres[i] = new Sphere(Vector3(3, (i - 4), 0), (float)rand() / RAND_MAX * 2);
	//	//spheres[i] = new Sphere();
	//}

	HitableList list(spheres, 2);

	Color(&film, &list);
	film.ToPPM();
	for (int i = 0; i < 2; ++i)
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
