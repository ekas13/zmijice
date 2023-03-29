#include "Snake.h"


Snake::Snake(Point2d headPosition, int index) :SnakeAIBase(headPosition, index) {

	this->score = 0;
}

Action Snake::step(std::vector<std::vector<int>> map)
{
	return SnakeAIBase::step(map);
}

//void Snake::addScore()
//{
//	this->score++;
//}

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