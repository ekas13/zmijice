#include "Point2d.h"
#include <math.h>

int Point2d::getX()
{
    return this->x;
}

int Point2d::getY()
{
    return this->y;
}

Point2d Point2d::operator-(Point2d other)
{
    return Point2d(x - other.x, y - other.y);
}

Point2d Point2d::operator+(Point2d other)
{
    return Point2d(x + other.x, y + other.y);
}

Point2d Point2d::operator*(int scalar)
{
    return Point2d(x * scalar, y * scalar);
}

bool Point2d::operator==(Point2d other)
{
    return (x == other.x) && (y == other.y);
}

float Point2d::getAngle()
{
    return atan2(y, x);
}

Point2d::Point2d(int x, int y)
{
    this->x = x;
    this->y = y;
};

Point2d::Point2d()
{
    this->x = 0;
    this->y = 0;
};

void Point2d::setLocation(int x, int y)
{
    this->x = x;
    this->y = y;
}

void Point2d::setLocation(Point2d p)
{
    this->x = p.x;
    this->y = p.y;
}