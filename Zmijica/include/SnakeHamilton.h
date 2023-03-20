#pragma once

#include <vector>
#include "SnakeBase.h"

class SnakeHamilton : public SnakeBase {

private:
    int mapSize;
    int wrongWay;
    bool downwardSnake();

public:
    SnakeHamilton(Point2d headPosition, int index, int mapSize);
    Action step(std::vector<std::vector<int>> map) override;
};