#pragma once
#include <vector>
#include <memory>
#include "Point2d.h"
#include "SnakeBase.h"
#include <string>

class Simulator 
{
private:
	std::vector<std::vector<int>> map;
	std::vector<std::shared_ptr<SnakeBase>> liveSnakes;
	std::vector<std::shared_ptr<SnakeBase>> deadSnakes;
	bool hasApple;
	unsigned int mapSize;
	void addSnake(std::string type, Point2d startPosition);
	int simulatorScore;
	Action getSnakeStep(int snakeIndex);

public:
	Simulator(unsigned int mapSize, std::vector<SnakeBase*> snakes = std::vector<SnakeBase*>());
	/// <returns>Map size in number of cells per row/column</returns>
	unsigned int getMapSize();

	/// <returns>Map matrix as vector of vectors</returns>
	std::vector<std::vector<int>> getMap();

	std::vector<std::shared_ptr<SnakeBase>>& getLiveSnakes();
	std::vector<std::shared_ptr<SnakeBase>>& getDeadSnakes();
	int getCumulativeSimulatorScore();
	int getNoOfDeadSnakes();

	/// returns 1 if simulator has at least one live snake 
	bool step();
};