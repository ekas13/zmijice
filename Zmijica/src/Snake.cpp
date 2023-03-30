#include "Snake.h"


Snake::Snake(Point2d headPosition, int index) :SnakeAIBase(headPosition, index) {

	this->score = 0;
	this->stepCnt = 0;
}

Action Snake::step(std::vector<std::vector<int>> map)
{
	this->stepCnt++;
	return SnakeAIBase::step(map);
}

//void Snake::addScore()
//{
//	this->score++;
//}
int Snake::getSteps()
{
	return this->stepCnt;
}
Snake::~Snake()
{
	//free;
	//dummy destruktor za potrebe izvedenog destruktora
}

void Snake::cross(SnakeAIBase& other, std::string typeOfCross)
{

}

void Snake::mutate(float chanceOfMutation)
{

}

void Snake::saveSnakeData()
{

}