#pragma once

#include "Point2d.h"
#include "Action.h"
#include <vector>
#include <list>

class SnakeBase
{
protected:
    std::list<Point2d> snakeCells;
    int index;
    int score;

public:
    SnakeBase(Point2d headPosition, int index);
    virtual ~SnakeBase();
    int getIndex();
    virtual void addScore();
    void pushFront(Point2d newHeadPosition);
    void popBack();
    int getScore();
    std::list<Point2d> getSnakeCells();
    virtual Action step(std::vector<std::vector<int>> map) = 0;
};