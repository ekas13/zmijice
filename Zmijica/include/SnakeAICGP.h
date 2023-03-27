#pragma once

#include <vector>
#include <string>
#include "SnakeBase.h"
#include "CGP.h"
#include "SnakeAIBase.h"

class SnakeAICGP : public SnakeAIBase
{
private:
    CGP* model;
    float outputBound;
    Action mapToAction(float output);
    void saveSnakeData();

public:
    SnakeAICGP(Point2d headPosition, int index, int numOfRows, int numOfCols, int numOfFunctions, int numOfFunctionArgs);
    SnakeAICGP(Point2d headPosition, int index, const SnakeAICGP& oldSnake);
    SnakeAICGP(Point2d headPosition, int index, std::string path);
    void cross(SnakeAIBase& other, std::string typeOfCross);
    void mutate(float chanceOfMutation);
    Action step(std::vector<std::vector<int>> map);
    virtual ~SnakeAICGP();
};