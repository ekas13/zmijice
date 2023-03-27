#pragma once
#include <vector>
#include <memory>
#include "Point2d.h"
#include "SnakeBase.h"
#include <string>
#include "Team.h"

class TeamSimulator
{
private:
	std::vector<std::vector<int>> map;
	std::vector<Team*> teams;
	std::vector<SnakeBase*> liveSnakes;
	bool hasApple;
	unsigned int mapSize;

public:
	TeamSimulator(unsigned int mapSize, std::vector<Team*> teams);
	/// <returns>Map size in number of cells per row/column</returns>
	unsigned int getMapSize();

	/// <returns>Map matrix as vector of vectors</returns>
	std::vector<std::vector<int>> getMap();

	std::vector<Team*> getTeams();

	bool step();
};
