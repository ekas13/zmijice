#include "SnakeBase.h"
#include "Config.h"

// 2d tocka (x i y koordinate)

// apstraktna klasa SnakeBase
SnakeBase::SnakeBase(Point2d headPosition, int index)
{
    this->index = index;
    this->snakeCells.push_back(headPosition);

    // pocetak: glava zmije je u centru mape, tijelo je lijevo od glave
    for (int i = 1; i < Config::snakeInitSize; i++)
    {
        this->snakeCells.push_back(
            Point2d(headPosition.getX() - i, headPosition.getY()));
    }

    this->score = Config::snakeInitSize;
}

SnakeBase::~SnakeBase()
{
    //dummy za brisanje nasljedenih
}

int SnakeBase::getIndex()
{
    return index;
}

void SnakeBase::addScore()
{
    this->score++;
}

int SnakeBase::getScore()
{
    return score;
}

void SnakeBase::pushFront(Point2d newHeadPosition)
{
    this->snakeCells.push_front(newHeadPosition);
};

void SnakeBase::popBack()
{
    this->snakeCells.pop_back();
}

std::list<Point2d> SnakeBase::getSnakeCells()
{
    return this->snakeCells;
}