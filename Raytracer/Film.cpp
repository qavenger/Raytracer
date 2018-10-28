#include "pch.h"
#include "Film.h"
#include <fstream>
#include "Vector.h"
Film::Film(uint32 width, uint32 height)
	:m_width(width), m_height(height)
{
	m_filmData = new Vector3[width * height];
}

void Film::ToPPM() const
{
	std::ofstream fout("OutImage.ppm", std::ios::out);
	fout << "P3" << m_width << ' ' << m_height << '\n' << 255 << '\n';
	for (uint32 y = 0; y < m_height; ++y)
	{
		uint32 heightOffset = y * m_width;
		for (uint32 x = 0; x < m_width; ++x)
		{
			uint32 idx = heightOffset + x;
			Vector3 data = m_filmData[idx];
			int ir = int(255.99f * sqrt(data.x()));
			int ig = int(255.99f * sqrt(data.y()));
			int ib = int(255.99f * sqrt(data.z()));
			fout << ir << ' ' << ig << ' ' << ib << ' ' << ' ';
		}
		fout << '\n';
	}
}

Film::~Film()
{
	delete[] m_filmData;
}

uint32 Film::GetWidth() const
{
	return m_width;
}

uint32 Film::GetHeight() const
{
	return m_height;
}

Vector3 * Film::GetFilmData()
{
	return m_filmData;
}
