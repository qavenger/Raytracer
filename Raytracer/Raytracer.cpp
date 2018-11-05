// Raytracer.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include "Film.h"
#include "Ray.h"
#include "HitableList.h"
#include "Sphere.h"
#include "Camera.h"
#include "Material.h"
#include "Random.h"

#include <thread>

#define NUM_OF_OBJ 5
#define WIDTH (178*2)
#define HEIGHT (100*2)
const int32 SAMPLE_COUNT = 100;
const int32 MAX_SCATTER_DEPTH = 50;
Camera cam(float3(-1, 0, 1), float3(1, 0, 0), float3(0, 0, 1), 90.0f, (float)WIDTH / HEIGHT);
Film film(WIDTH, HEIGHT);
Hitable* spheres[NUM_OF_OBJ];

Lambertian lam(Vector3(0.38f, 0.42f, 0.5f));
Metal metal0(Vector3(0.8f), 0.5f);
Metal metal1(Vector3(0.8f, 0.6f, 0.2f), 0.1f);
Dielectric diel(1.5f);

Vector3 trace(const Ray& ray, const HitableList* list, int depth)
{
	HitInfo hit;
	if (list->intersectRay(ray, &hit))
	{
		Vector3 attenuation;
		Ray scatteredRay;
		if (depth < MAX_SCATTER_DEPTH && hit.HitMat->Scatter(ray, hit, attenuation, scatteredRay))
		{
			return trace(scatteredRay, list, depth + 1) * attenuation;
		}
		return 0.0f;
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
		float t = 0.5f * (ray.Direction().z + 1);
		return lerp(Vector3(1), Vector3(0.5f, 0.7f, 1.0f), t);;
	}
}

void Loop(Film* film, const HitableList* list, uint32 xStart, uint32 yStart, uint32 xEnd, uint32 yEnd)
{
	float inv_w = 1.0f / film->GetWidth();
	float inv_h = 1.0f / film->GetHeight();
	for (uint32 y = yStart; y < yEnd; ++y)
	{
		for (uint32 x = xStart; x < xEnd; ++x)
		{
			uint32 index = y * film->GetWidth() + x;
			Vector3 c;
			for (int s = 0; s < SAMPLE_COUNT; ++s)
			{
				float u = (x + Random::Next())* inv_w;
				float v = (y + Random::Next())* inv_h;
				Ray ray = cam.GetRay(u, v);
				//std::cout << ray.Direction() << std::endl;
				c += trace(ray, list, 0);
			}
			film->GetFilmData()[index] = c / SAMPLE_COUNT;
			//int32 cur = (int32)(((float)index / totalPix) * 100);
			//if (cur > percentage)
			//{
			//	percentage = cur;
			//	std::cout << percentage << '\n';
			//}
		}
	}
}

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

	uint32 maxThread = std::thread::hardware_concurrency();
	std::thread** threads;
	uint32 HeightStep = film->GetHeight() / maxThread;
	threads = new std::thread*[maxThread];
	uint32 i = 0;
	for (; i < maxThread - 1; ++i)
	{
		threads[i] = new std::thread(Loop, film, list, 0, i * HeightStep, film->GetWidth(), (i + 1) * HeightStep);
	}
	threads[i] = new std::thread(Loop, film, list, 0, i * HeightStep, film->GetWidth(), film->GetHeight());
	/*std::thread t1(Loop, film, list, 0, 0 * HeightStep, film->GetWidth(), 1 * HeightStep);
	std::thread t2(Loop, film, list, 0, 1 * HeightStep, film->GetWidth(), 2 * HeightStep);
	std::thread t3(Loop, film, list, 0, 2 * HeightStep, film->GetWidth(), 3 * HeightStep);*/

	for (i = 0; i < maxThread; ++i)
	{
		threads[i]->join();
	}
	std::thread t;

	for (i = 0; i < maxThread; ++i)
	{
		delete threads[i];
	}
	delete[]threads;

	/*t1.join();
	t2.join();
	t3.join();*/
	//Loop(film, list, 0, 0, film->GetWidth(), film->GetHeight());
	/*int32 totalPix = film->GetHeight() * film->GetWidth();
	int32 percentage = 0;*/

	//for (uint32 y = 0; y < film->GetHeight(); ++y)
	//{
	//	for (uint32 x = 0; x < film->GetWidth(); ++x)
	//	{
	//		uint32 index = y * film->GetWidth() + x;
	//		// x==57&&y==32
	//		Vector3 c;
	//		//float validCount = 0;
	//		for (int s = 0; s < SAMPLE_COUNT; ++s)
	//		{
	//			float u = (x + Random::Next())* inv_w;
	//			float v = (y + Random::Next())* inv_h;
	//			Ray ray = cam.GetRay(u, v);
	//			/*Vector3 tmpC = trace(ray, list, 0);
	//			if (!tmpC.isnan())
	//			{
	//				c += tmpC;
	//				++validCount;
	//			}*/
	//			c += trace(ray, list, 0);
	//		}
	//		
	//		film->GetFilmData()[index] = c / SAMPLE_COUNT;
	//		int32 cur = (int32)(((float)index / totalPix) * 100);
	//		if (cur > percentage)
	//		{
	//			percentage = cur;
	//			std::cout << percentage << '\n';
	//		}
	//	}
	//}
}


#include <time.h>
#include "FilmExporter.h"
int main()
{
	/*float a;
	for (int i = 0; i< 100000; ++i)
	{
		a = drand();
		if(a >= 1) 
		std::cout << a << '\n';
	}
	return 0;*/
	
	uint64_t t = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
	srand((unsigned int)time(0));
	spheres[0] = new Sphere(Vector3(6, 0, 0), 2);
	spheres[0]->m_material = &lam;
	spheres[1] = new Sphere(Vector3(6, 0, -102), 100);
	spheres[1]->m_material = &metal1;
	spheres[2] = new Sphere(Vector3(5.5f, 4, 0), 2);
	spheres[2]->m_material = &metal0;
	spheres[3] = new Sphere(Vector3(5.5f, -4, 0), 2);
	spheres[3]->m_material = &diel;
	spheres[4] = new Sphere(Vector3(5.5f, -4, 0), -1.95f);
	spheres[4]->m_material = &diel;
	HitableList list(spheres, NUM_OF_OBJ);
	Color(&film, &list);
	FilmExporter::ToPPM(&film);
	for (int i = 0; i < NUM_OF_OBJ; ++i)
	{
		delete spheres[i];
	}
	t = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count() - t;
	std::cout << t / 1000 << "s\n";
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
