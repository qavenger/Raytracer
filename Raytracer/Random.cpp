#include "pch.h"
#include "Random.h"

std::default_random_engine Random::engine;
std::uniform_real_distribution<float> Random::dist;

float Random::Next()
{
	return dist(engine);
}
