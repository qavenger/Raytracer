#pragma once

class Film
{
public:
	Film(uint32 width, uint32 height);
	void ToPPM() const;
	~Film();
public:
	uint32 GetWidth()const;
	uint32 GetHeight()const;
	class Vector3* GetFilmData();
private:
	uint32 m_width, m_height;
	class Vector3* m_filmData;
};

