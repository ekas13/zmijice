#pragma once
#include <vector>
#include <memory>
#include "Point2d.h"
#include "SnakeBase.h"
#include "SnakeAIBase.h"
#include <string>
#include "GP.h"
#include "CGP.h"
#include "Snake.h"

class Team
{
private:
	GP* model;
	//CGP* model;
	std::vector<std::shared_ptr<SnakeBase>> snakes;
	std::vector<std::shared_ptr<SnakeBase>> deadSnakes;
	int teamScore;
	std::vector<Action> mapToActions(float given_output);
	//std::vector<Action> agentsStep(std::vector<Action> steps);
public:
	Team();//unsigned int mapSize, std::vector<SnakeAIBase*> snakes);
	Team(const Team& oldTeam);
	std::vector<Action> step(std::vector<std::vector<int>> map);
	void cross(Team& other, std::string typeOfCross);
	void mutate(float chanceOfMutation);
	int getTeamScore();
	void setTeamScore(int score);
	int getNoOfAliveSnakes();
	void addSnake(std::shared_ptr<SnakeBase> snake);
	void addToTeamScore(int value);
	void removeSnakeAt(int index);
	void addDeadSnake(std::shared_ptr<SnakeBase> snake);
	std::vector<std::shared_ptr<SnakeBase>>& getLiveSnakes();
	std::vector<std::shared_ptr<SnakeBase>>& getDeadSnakes();
	int getFirstSnakeScore();
	int getSecondSnakeScore();
	std::vector<int> allSnakeScores();
	std::vector<int> allSnakeSteps();
};