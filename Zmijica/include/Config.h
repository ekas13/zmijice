#pragma once

#include "Saveable.h"
#include <string>

struct Config : public Saveable
{
	static std::string mode;
	static std::string playModePlayer1;
	static std::string playModePlayer2;
	static std::string playModePlayer3;
	static std::string playModePlayer4;
	static float playSpeed;
	static unsigned int mapSize;
	static unsigned int snakeInitSize;
	static unsigned int populationSize;
	static unsigned int snakesPerSim;
	static std::string selectionType;
	static unsigned int selectionSize;
	static std::string crossType;
	static float mutationChance;
	static unsigned int hiddenLayerDepth;
	static unsigned int hiddenLayerWidth;
	static std::string activationFunction;
	static std::string AIModel;

	//CGP
	static int numOfRows;
	static int numOfCols;
	static int numOfFunctions;
	static int numOfFunctionArgs;

	//GP
	static int maxDepth;

	Config(std::string path);

private:
	void saveData() override;
};