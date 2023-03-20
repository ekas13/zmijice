#pragma once
#include <vector>
#include "SnakeAIBase.h"
#include "SnakeBase.h"
#include "Point2d.h"
#include "Config.h"
#include <math.h>
#include <stdexcept>

#define DIRECTIONS 7
#define minDis 2

std::vector<std::vector<int>> SnakeAIBase::calculateDirections() {
    Point2d snakeHead = snakeCells.front();
    int head_x = snakeHead.getX();
    int head_y = snakeHead.getY();

    auto iterator = snakeCells.begin();
    std::advance(iterator, 1);
    Point2d secondCell = *iterator;
    int body_x = secondCell.getX();
    int body_y = secondCell.getY();

    if (head_x == body_x && head_y - 1 == body_y) {
        std::vector<std::vector<int>> directions{{-1,-1}, {-1,0}, {-1,1}, {0,1}, {1,1}, {1,0}, {1,-1}};
        return directions;
    }
    if (head_x == body_x && head_y + 1 == body_y) {
        std::vector<std::vector<int>> directions{{1,1}, {1,0}, {1,-1}, {0,-1}, {-1,-1}, {-1,0}, {-1,1}};
        return directions;
    }
    if (head_x - 1 == body_x && head_y == body_y) {
        std::vector<std::vector<int>> directions{{-1,1}, {0,1}, {1,1}, {1,0}, {1,-1}, {0,-1}, {-1,-1}};
        return directions;
    }
    if (head_x + 1 == body_x && head_y == body_y) {
        std::vector<std::vector<int>> directions{{1,-1}, {0,-1}, {-1,-1}, {-1,0}, {-1,1}, {0,1}, {1,1}};
        return directions;
    }
}

std::vector<float> SnakeAIBase::calculateNearestDistance(std::vector<int> direction, std::vector<std::vector<int>> map) {
    Point2d snakeHead = snakeCells.front();
    int head_x = snakeHead.getX();
    int head_y = snakeHead.getY();
    int curr_x = head_x;
    int curr_y = head_y;

    do {
        curr_x = curr_x + direction[0];
        curr_y = curr_y + direction[1];

        bool outOfBounds = curr_x < 0 || curr_x == Config::mapSize || curr_y < 0 || curr_y == Config::mapSize;
        if (outOfBounds) {
            float distance = sqrt((curr_x-head_x)*(curr_x-head_x) + (curr_y-head_y)*(curr_y-head_y));

            //std::vector<float> result = {distance, static_cast<float>(-1)};
            std::vector<float> result = {distance < minDis ? distance : 0, static_cast<float>(distance < minDis ? -1 : 0) };
            //std::vector<float> result = {-distance};

            return result;
        }

        int currentCell = map[curr_x][curr_y];
        if (currentCell != 0) {
            float distance = sqrt((curr_x-head_x)*(curr_x-head_x) + (curr_y-head_y)*(curr_y-head_y));

            //std::vector<float> result = {distance, static_cast<float>(currentCell == 1 ? 1 : -1)};
            std::vector<float> result = {currentCell == 1 ? distance : distance < minDis ? distance : 0, static_cast<float>(currentCell == 1 ? 1 : distance < minDis ? -1 : 0)};
            //std::vector<float> result = {currentCell == 1 ? distance : -distance};

            if (currentCell == 1)
            {
                distanceToApple = distance;
            }
            return result;
        }
    } while (map[curr_x][curr_y] == 0);
}

std::vector<float> SnakeAIBase::getInputsRaycast7(std::vector<std::vector<int>> map) {
    std::vector<float> inputs;
    std::list<Point2d> snakeCells = getSnakeCells();
    Point2d snakeHead = snakeCells.front();

    std::vector<std::vector<int>> directions = this->calculateDirections();

    for (int i=0; i<DIRECTIONS; i++) {
        std::vector<int> currentDirection = directions[i];
        std::vector<float> nearestObject = this->calculateNearestDistance(currentDirection, map);
        inputs.push_back(nearestObject[0]);
        inputs.push_back(nearestObject[1]);
    }

    return inputs;
}

void SnakeAIBase::saveData()
{
    saveSnakeData();

    save("visionType", visionType);
}

std::vector<float> SnakeAIBase::getInputs(std::vector<std::vector<int>> map)
{
    if (visionType == "raycast7")
        return getInputsRaycast7(map);
}

unsigned int SnakeAIBase::getNumberOfInputs() const
{
    return numInputs;
}

SnakeAIBase::SnakeAIBase(Point2d headPosition, int index) : SnakeBase(headPosition, index)
{
    stepsSinceLastApple = 0;
    distanceToApple = Config::mapSize;

    this->visionType = "raycast7";
    numInputs = 14;
}

SnakeAIBase::SnakeAIBase(Point2d headPosition, int index, std::string path) : SnakeBase(headPosition, index), Saveable(path)
{
    stepsSinceLastApple = 0;
    distanceToApple = Config::mapSize;

    std::string vType;
    load("visionType", vType);
    setVisionType(vType);
}

void SnakeAIBase::setVisionType(std::string type)
{
    visionType = type;
    if (type == "raycast7")
        numInputs = 14;
    else
        throw std::invalid_argument("Vision type: " + type + " is invalid.");
}

SnakeAIBase::~SnakeAIBase()
{
    //dummy destruktor za potrebe izvedenog destruktora
}

Action SnakeAIBase::step(std::vector<std::vector<int>> map)
{
    stepsSinceLastApple++;
    return STRAIGHT;
}

void SnakeAIBase::addScore()
{
    distanceToApple = Config::mapSize;
    SnakeBase::addScore();
    stepsSinceLastApple = 0;
}

int SnakeAIBase::getStepsSinceLastApple()
{
    return stepsSinceLastApple;
}

int SnakeAIBase::getDistanceToApple()
{
    return distanceToApple;
}
