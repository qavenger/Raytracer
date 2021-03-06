// Raytracer.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include "Film.h"
#include "Ray.h"
#include "HitableList.h"
#include "BVH.h"
#include "Sphere.h"
#include "MovingSphere.h"
#include "Camera.h"
#include "Material.h"
#include "Random.h"
#define USE_THREADING 1
#define USE_BVH 1

#if USE_THREADING
#include <thread>
#else
#include <chrono>
#endif

#define NUM_OF_OBJ 5
#define WIDTH (178)
#define HEIGHT (100)
const int32 SAMPLE_COUNT = 100;
const int32 MAX_SCATTER_DEPTH = 50;
const float3 origin(-3, 2, 2);
const float3 lookat(0, 0, 0);
float focal_length = length(lookat - origin);
float aperture = .0f;
Camera cam(origin, lookat, float3(0, 0, 1.f), 75, (float)WIDTH / HEIGHT, focal_length, aperture, 0.5f);
Film film(WIDTH, HEIGHT);
//Hitable* spheres[NUM_OF_OBJ];
int counter = 0;
Lambertian lam(Vector3(0.38f, 0.42f, 0.5f));
Lambertian mat_ground(Vector3(0.5f));
Metal metal0(Vector3(0.8f), 0.5f);
Metal metal1(Vector3(0.8f, 0.6f, 0.2f), 0.1f);
Dielectric diel(1.5f);

Vector3 trace(const Ray& ray, const Hitable* list, int depth)
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

void Loop(Film* film, const Hitable* list, uint32 xStart, uint32 yStart, uint32 xEnd, uint32 yEnd)
{
	float inv_w = 1.0f / film->GetWidth();
	float inv_h = 1.0f / film->GetHeight();
	float inv_sc = 1 / (float)SAMPLE_COUNT;
	int total = xEnd * yEnd;
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
			film->GetFilmData()[index] = c * inv_sc;
			//int32 cur = (int32)(((float)index / totalPix) * 100);
			//if (cur > percentage)
			//{
			//	percentage = cur;
			//	std::cout << percentage << '\n';
			//}
			++counter;
#if !USE_THREADING
			system("cls");
			printf("%d / %d\n", counter, total);
#endif
		}
	}
}

void Capture(Film* film, const Hitable* list)
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
#if USE_THREADING
	uint32 maxThread = std::thread::hardware_concurrency();
	std::cout << "Total Threads: " << maxThread << std::endl;
	std::thread** threads;
	uint32 HeightStep = film->GetHeight() / maxThread;
	threads = new std::thread*[maxThread];
	uint32 i = 0;
	for (; i < maxThread - 1; ++i)
	{
		threads[i] = new std::thread(Loop, film, list, 0, i * HeightStep, film->GetWidth(), (i + 1) * HeightStep);
	}
	threads[i] = new std::thread(Loop, film, list, 0, i * HeightStep, film->GetWidth(), film->GetHeight());
	int total = WIDTH * HEIGHT;
	while (counter < total)
	{
		system("cls");
		printf("%d / %d\n", counter , total);
	}
	for (i = 0; i < maxThread; ++i)
	{
		threads[i]->join();
	}
	//std::thread t;

	for (i = 0; i < maxThread; ++i)
	{
		delete threads[i];
	}
	delete[]threads;
#else
	Loop(film, list, 0, 0, film->GetWidth(), film->GetWidth());
#endif

}
#if USE_BVH
int RandomScene(int numRow, int numCol, Hitable**& list) {
	int total = numRow * numCol;
	assert(total >= 5);
	/*Hitable** list = new Hitable*[total + 5];
	list[0] = new Sphere(Vector3(0, 0, -1000), 1000);
	list[0]->m_material = &mat_ground;
	list[1] = new Sphere(Vector3(0, 0, 1), 1);
	list[1]->m_material = &lam;
	list[2] = new Sphere(Vector3(0, 4, 1), 1);
	list[2]->m_material = &metal0;
	list[3] = new Sphere(Vector3(0, -4, 1), 1);
	list[3]->m_material = &diel;
	list[4] = new Sphere(Vector3(0, -4, 1), -0.95f);
	list[4]->m_material = &diel;*/
	//int i;
	list = new Hitable*[numRow*numCol * 4 + 5];

	//HitableList* hitablelist = new HitableList();
	
	//hitablelist->Add(new Sphere(Vector3(0, 0, -1000), 1000, &mat_ground));
	//hitablelist->Add(new Sphere(Vector3(0, 0, 1), 1, &lam));
	//hitablelist->Add(new Sphere(Vector3(0, 3, 1), 1, &metal0));
	//hitablelist->Add(new Sphere(Vector3(0, -3, 1), 1, &diel));
	//hitablelist->Add(new Sphere(Vector3(0, -3, 1), -0.95f, &diel));

	list[0] = new Sphere(Vector3(0, 0, -1000), 1000, &mat_ground);
	list[1] = new Sphere(Vector3(0, 0, 1), 1, &lam);
	list[2] = new Sphere(Vector3(0, 3, 1), 1, &metal0);
	list[3] = new Sphere(Vector3(0, -3, 1), 1, &diel);
	list[4] = new Sphere(Vector3(0, -3, 1), -0.95f, &diel);
	int i = 5;
	for (int y = 0; y < numRow * 2; ++y)
	{
		for (int x = 0; x < numCol * 2; ++x)
		{
			float mat = Random::Next();
			float3 center(x - numRow + 0.4f * Random::Next(), y - numCol + 0.8f * Random::Next(), 0.2f);
			if (length(center) >1)
			{
				//list[i++] = new Sphere(center, 0.2f);
				if (mat < 0.8f)
				{
					MovingSphere* s = new MovingSphere(center, 0.2f, new Lambertian(float3(Random::Next(), Random::Next(), Random::Next())));
					s->SetVelocity(float3(0, 0, Random::Next()));
					//hitablelist->Add(s);
					list[i] = s;
					//list[i]->m_material = new Lambertian(float3(Random::Next(), Random::Next(), Random::Next()));
				}
				else if (mat < 0.95f)
				{
					//hitablelist->Add(new Sphere(center, 0.2f, new Metal((float3(Random::Next(), Random::Next(), Random::Next()) + 1.0f) *0.5f, 0.5f * Random::Next())));
					list[i] = new Sphere(center, 0.2f, new Metal((float3(Random::Next(), Random::Next(), Random::Next()) + 1.0f) *0.5f, 0.5f * Random::Next()));
					//list[i]->m_material = new Metal(float3(Random::Next(), Random::Next(), Random::Next()) *0.5f + 1.0f, 0.5f * Random::Next());
				}
				else
				{
					//hitablelist->Add(new Sphere(center, 0.2f, new Dielectric(1.5f)));
					list[i] = new Sphere(center, 0.2f, new Dielectric(1.5f));
					//list[i]->m_material = new Dielectric(1.5f);
				}
				++i;
			}
		}
	}
	return i;
	//return hitablelist;
}
#else
Hitable* RandomScene(int numRow, int numCol) {
	int total = numRow * numCol;
	assert(total >= 5);
	/*Hitable** list = new Hitable*[total + 5];
	list[0] = new Sphere(Vector3(0, 0, -1000), 1000);
	list[0]->m_material = &mat_ground;
	list[1] = new Sphere(Vector3(0, 0, 1), 1);
	list[1]->m_material = &lam;
	list[2] = new Sphere(Vector3(0, 4, 1), 1);
	list[2]->m_material = &metal0;
	list[3] = new Sphere(Vector3(0, -4, 1), 1);
	list[3]->m_material = &diel;
	list[4] = new Sphere(Vector3(0, -4, 1), -0.95f);
	list[4]->m_material = &diel;*/
	//int i;
	//list = new Hitable*[numRow*numCol * 4 + 5];

	HitableList* hitablelist = new HitableList();

	hitablelist->Add(new Sphere(Vector3(0, 0, -1000), 1000, &mat_ground));
	hitablelist->Add(new Sphere(Vector3(0, 0, 1), 1, &lam));
	hitablelist->Add(new Sphere(Vector3(0, 3, 1), 1, &metal0));
	hitablelist->Add(new Sphere(Vector3(0, -3, 1), 1, &diel));
	hitablelist->Add(new Sphere(Vector3(0, -3, 1), -0.95f, &diel));

	/*list[0] = new Sphere(Vector3(0, 0, -1000), 1000, &mat_ground);
	list[1] = new Sphere(Vector3(0, 0, 1), 1, &lam);
	list[2] = new Sphere(Vector3(0, 3, 1), 1, &metal0);
	list[3] = new Sphere(Vector3(0, -3, 1), 1, &diel);
	list[4] = new Sphere(Vector3(0, -3, 1), -0.95f, &diel);*/
	int i = 5;
	for (int y = 0; y < numRow * 2; ++y)
	{
		for (int x = 0; x < numCol * 2; ++x)
		{
			float mat = Random::Next();
			float3 center(x - numRow + 0.4f * Random::Next(), y - numCol + 0.8f * Random::Next(), 0.2f);
			if (length(center) >1)
			{
				//list[i++] = new Sphere(center, 0.2f);
				if (mat < 0.8f)
				{
					MovingSphere* s = new MovingSphere(center, 0.2f, new Lambertian(float3(Random::Next(), Random::Next(), Random::Next())));
					s->SetVelocity(float3(0, 0, Random::Next()));
					hitablelist->Add(s);
					//list[i] = s;
					//list[i]->m_material = new Lambertian(float3(Random::Next(), Random::Next(), Random::Next()));
				}
				else if (mat < 0.95f)
				{
					hitablelist->Add(new Sphere(center, 0.2f, new Metal((float3(Random::Next(), Random::Next(), Random::Next()) + 1.0f) *0.5f, 0.5f * Random::Next())));
					//list[i] = new Sphere(center, 0.2f, new Metal((float3(Random::Next(), Random::Next(), Random::Next()) + 1.0f) *0.5f, 0.5f * Random::Next()));
					//list[i]->m_material = new Metal(float3(Random::Next(), Random::Next(), Random::Next()) *0.5f + 1.0f, 0.5f * Random::Next());
				}
				else
				{
					hitablelist->Add(new Sphere(center, 0.2f, new Dielectric(1.5f)));
					//list[i] = new Sphere(center, 0.2f, new Dielectric(1.5f));
					//list[i]->m_material = new Dielectric(1.5f);
				}
				++i;
			}
		}
	}
	//return i;
	return hitablelist;
}
#endif
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
	
	srand((unsigned int)time(0));
	/*spheres[0] = new Sphere(Vector3(6, 0, 0), 2);
	spheres[0]->m_material = &lam;
	spheres[1] = new Sphere(Vector3(6, 0, -102), 100);
	spheres[1]->m_material = &metal1;
	spheres[2] = new Sphere(Vector3(3.f, 4, 0), 2);
	spheres[2]->m_material = &metal0;
	spheres[3] = new Sphere(Vector3(8.5f, -4, 0), 2);
	spheres[3]->m_material = &diel;
	spheres[4] = new Sphere(Vector3(8.5f, -4, 0), -1.95f);
	spheres[4]->m_material = &diel;*/
#if !USE_BVH
	Hitable* list = RandomScene(100, 100);

	uint64_t t = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
	Capture(&film, list);
#else
	Hitable** list;
	int n = RandomScene(100, 100, list);
	BVH bvh(list, n);

	uint64_t t = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
	Capture(&film, &bvh);
#endif
	//HitableList list(spheres, NUM_OF_OBJ);
	//Capture(&film, list);
	
	FilmExporter::ToPPM(&film);
	
	delete list;
	t = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count() - t;
	std::cout << t / 1000 << "s\n";
	system("pause");
}
