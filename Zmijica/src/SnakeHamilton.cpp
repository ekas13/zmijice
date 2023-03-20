#include "SnakeHamilton.h"
#include "SnakeBase.h"

SnakeHamilton::SnakeHamilton(Point2d headPosition, int index, int mapSize) : SnakeBase(headPosition, index)
{
    this->mapSize = mapSize;
    if (!(mapSize % 2 == 0 && mapSize > 1))
        throw std::exception("mapSize must be even number to have hamiltonian path!");

    int wrongWay = 0;

};

Action SnakeHamilton::step(std::vector<std::vector<int>> map)
{
    int xCoord = snakeCells.front().getX();
    int yCoord = snakeCells.front().getY();


    if (yCoord % 2 == 1 && downwardSnake() == 1) {
        wrongWay = 2;
    }
    if (wrongWay > 0)
    {
        if (wrongWay == 2) {
            wrongWay--;
            return RIGHT;
        }
        if (wrongWay == 1) {
            wrongWay--;
            return LEFT;
        }
    }
    else if (xCoord == 0 && (yCoord == 0 || yCoord == mapSize - 1))
        return LEFT;
    else if (xCoord == 1 && !(yCoord == 0 || yCoord == mapSize - 1))
        return RIGHT;
    else if (xCoord == mapSize - 1)
        return LEFT;
    else
        return STRAIGHT;

};

bool SnakeHamilton::downwardSnake()
{
    int headXCoord = snakeCells.front().getX();
    std::list<Point2d>::iterator it = snakeCells.begin();
    std::advance(it, 1);
    Point2d body = *it;
    int bodyXCoord = body.getX();
    if (headXCoord > bodyXCoord)
        return true;
    else
        return false;
}