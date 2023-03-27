#pragma once
#include <vector>
#include <memory>
#include "Point2d.h"
#include "SnakeBase.h"
#include "SnakeAIBase.h"
#include <string>
#include "GP.h"

class Team
{
private:
	GP* model;
	std::vector<SnakeBase> snakes;
	int teamScore;
	std::vector<Action> mapToActions(float output);
	std::vector<Action> agentsStep(std::vector<Action> steps);
	 
public:
	Team();//unsigned int mapSize, std::vector<SnakeAIBase*> snakes);
	Team(const Team& oldTeam);
	std::vector<Action> step(std::vector<std::vector<int>> map);
	void cross(Team& other, std::string typeOfCross);
	void mutate(float chanceOfMutation);
	int getTeamScore();
	void setTeamScore(int score);
	int getNoOfAliveSnakes();
	void addSnake(SnakeBase* snake);
	void addTeamScore();
};