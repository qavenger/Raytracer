#pragma once
#include <random>
class Random
{
public:
	static float Next();
	static std::default_random_engine engine;
	static std::uniform_real_distribution<float> dist;
};

