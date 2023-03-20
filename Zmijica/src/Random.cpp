#include "Random.h"
#include <stdexcept>

std::random_device Random::rd;
std::mt19937 Random::gen(rd());

int Random::generateIntGeometric(float p)
{
	if (p <= 0 || p >= 1)
		throw std::invalid_argument("Probability of success must be between 0 and 1");

	std::geometric_distribution<> d(p);
	return d(gen);
}

int Random::generateInt(int lo, int hi)
{
	return (int)generateFloat(lo, hi);
}

float Random::generateFloat(float lo, float hi)
{
	if (hi < lo)
		throw std::invalid_argument("Second argument cannot be lower than first.");

	std::uniform_real_distribution<> dis(lo, hi);
	return dis(gen);
}