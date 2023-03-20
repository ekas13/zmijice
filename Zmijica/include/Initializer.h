#pragma once

class Simulator;

class Initializer
{
private:
	static void saveSimSnake(Simulator* sim);

public:
	static void initializeLearn();
	static void initializePlay();
};