#include "pch.h"
#include "FilmExporter.h"
#include <fstream>
#include "Film.h"
#include "Vector.h"

void FilmExporter::ToPPM(const Film * film)
{
	std::ofstream fout("OutImage.ppm", std::ios::out);
	uint32 width = film->GetWidth();
	uint32 height = film->GetHeight();

	const Vector3* filmData = film->GetFilmData();
	fout << "P3" << width << ' ' << height << '\n' << 255 << '\n';

	for (uint32 y = 0; y < height; ++y)
	{
		uint32 heightOffset = y * width;
		for (uint32 x = 0; x < width; ++x)
		{
			uint32 idx = heightOffset + x;
			Vector3 data = filmData[idx];
			int ir = int(255.99f * sqrt(data.x));
			int ig = int(255.99f * sqrt(data.y));
			int ib = int(255.99f * sqrt(data.z));
			if (ir < 0)
			{
				int i = 0;
			}
			fout << ir << ' ' << ig << ' ' << ib << ' ' << ' ';
		}
		fout << '\n';
	}
}
