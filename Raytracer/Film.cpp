#include "pch.h"
#include "Film.h"
#include <fstream>
#include "Vector.h"
Film::Film(uint32 width, uint32 height)
	:m_width(width), m_height(height)
{
	m_filmData = new Vector3[width * height];
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

const Vector3 * Film::GetFilmData()const
{
	return m_filmData;
}

Vector3 * Film::GetFilmData()
{
	return m_filmData;
}
