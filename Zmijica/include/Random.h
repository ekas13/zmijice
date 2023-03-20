#pragma once
#include <random>

class Random
{
private:
	static std::random_device rd;
	static std::mt19937 gen;
public:
	static int generateIntGeometric(float p);
	static int generateInt(int lo = 0, int hi = RAND_MAX);
	static float generateFloat(float lo = 0, float hi = 1);
};