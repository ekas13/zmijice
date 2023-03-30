#pragma once
#pragma once
#include <vector>
#include "SnakeAIBase.h"

class Snake : public SnakeAIBase {
private:
	int score;
	int stepCnt;
	std::vector<float> getInputsRaycast7(std::vector<std::vector<int>> map);
	void saveSnakeData();
	void cross(SnakeAIBase& other, std::string typeOfCross);
	void mutate(float chanceOfMutation);
public:
	~Snake();
	Snake(Point2d headPosition, int index);
	Action step(std::vector<std::vector<int>> map);
	int getSteps();
	//void addScore();
};