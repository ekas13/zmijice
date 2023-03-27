#pragma once
#include "SnakeBase.h"
#include "Direction.h"

class SnakeHuman : public SnakeBase
{
private:
	Action getActionFromDirections(Point2d snakeDir, Point2d userDir);
	static Direction userArrowInput;
	static bool userArrowInputConsumed;
public:
	SnakeHuman(Point2d headPosition, int index);
	Action step(std::vector<std::vector<int>> map) ;
	static void pushUserArrowInput(Direction d);
};