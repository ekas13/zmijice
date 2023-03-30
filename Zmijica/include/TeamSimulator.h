#pragma once
#include <vector>
#include <memory>
#include "Point2d.h"
#include "SnakeBase.h"
#include <string>
#include "Team.h"
#include "SimulatorBase.h"

class TeamSimulator 
{
private:
	std::vector<std::vector<int>> map;
	std::vector<std::shared_ptr<Team>> teams;
	std::vector<std::shared_ptr<SnakeBase>> liveSnakes;
	bool hasApple;
	unsigned int mapSize;
	int appleNo;
	int steps;
	int generation;

public:
	TeamSimulator(unsigned int mapSize, std::vector<Team*> teams,int generation);
	/// <returns>Map size in number of cells per row/column</returns>
	unsigned int getMapSize();

	/// <returns>Map matrix as vector of vectors</returns>
	std::vector<std::vector<int>> getMap();
	std::vector<std::shared_ptr<SnakeBase>>& getLiveSnakes();
	std::vector<std::shared_ptr<Team>>& getTeams();

	bool step();
};
