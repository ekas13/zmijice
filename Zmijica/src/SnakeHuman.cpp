#include "SnakeHuman.h"
#include "Config.h"
#include "Direction.h"

Action SnakeHuman::getActionFromDirections(Point2d snakeDir, Point2d userDir)
{
	if (snakeDir == userDir || snakeDir == userDir * -1)
		return STRAIGHT;

	Action result;
	Point2d snakeDirOpposite = snakeDir * -1;
	float snakeAngle = snakeDir.getAngle();
	float oppositeAngle = snakeDirOpposite.getAngle();
	float userAngle = userDir.getAngle();

	//Calculates if user direction is to the left or right of snake direction
	//and returns respective value
	if (snakeAngle > oppositeAngle)
	{
		if (userAngle < snakeAngle && userAngle > oppositeAngle)
			result = RIGHT;
		else
			result = LEFT;
	}
	else
	{
		if (userAngle < oppositeAngle && userAngle > snakeAngle)
			result = LEFT;
		else
			result = RIGHT;
	}

	return result;
}

SnakeHuman::SnakeHuman(Point2d headPosition, int index) : SnakeBase(headPosition, index)
{
	//Ovaj konstruktor sluzi za inicializaciju base konstruktora
}

Action SnakeHuman::step(std::vector<std::vector<int>> map)
{
	//Point2d used as vector of direction

	Direction userDir = userArrowInput;
	userArrowInputConsumed = true;
	Point2d snakeDirection = snakeCells.front() - *(++snakeCells.begin());
	Point2d userDirection;

	//Invert snake direction to match cartesian coordinates
	snakeDirection.setLocation(snakeDirection.getY(), snakeDirection.getX() * -1);

	switch (userDir)
	{
	case L:
		userDirection.setLocation(-1, 0);
		break;
	case U:
		userDirection.setLocation(0, 1);
		break;
	case R:
		userDirection.setLocation(1, 0);
		break;
	case D:
		userDirection.setLocation(0, -1);
		break;
	default:
		break;
	}

	return getActionFromDirections(snakeDirection, userDirection);
}

void SnakeHuman::pushUserArrowInput(Direction d)
{
	//Sets new input only if previous wasn't consumed
	if (userArrowInputConsumed)
		userArrowInput = d;
	userArrowInputConsumed = false;
}

Direction SnakeHuman::userArrowInput = D;
bool SnakeHuman::userArrowInputConsumed = true;