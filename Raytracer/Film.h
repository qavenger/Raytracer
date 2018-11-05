#pragma once
#include "Vector.h"
class Film
{
public:
	Film(uint32 width, uint32 height);
	~Film();
public:
	uint32 GetWidth()const;
	uint32 GetHeight()const;
	const Vector3* GetFilmData()const;
	Vector3* GetFilmData();
private:
	uint32 m_width, m_height;
	class Vector3* m_filmData;
};

