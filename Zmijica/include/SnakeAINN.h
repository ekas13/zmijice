#pragma once

#include <vector>
#include <string>
#include "SnakeBase.h"
#include "NN.h"
#include "SnakeAIBase.h"

class SnakeAINN: public SnakeAIBase
{
private:
    NN *model;
    int hiddenLayerDepth;
    int hiddenLayerWidth;
    std::string aFName;
    Action mapToAction(float output);
    void saveSnakeData() override;
public:
    SnakeAINN(Point2d headPosition, int index, int hiddenLayerDepth, int hiddenLayerWidth, std::string activationFunction);
    SnakeAINN(Point2d headPosition, int index, const SnakeAINN& oldSnake);
    SnakeAINN(Point2d headPosition, int index, std::string path);
    void cross(SnakeAIBase& other, std::string typeOfCross) override;
    void mutate(float chanceOfMutation) override;
    Action step(std::vector<std::vector<int>> map) override;
    virtual ~SnakeAINN();
};