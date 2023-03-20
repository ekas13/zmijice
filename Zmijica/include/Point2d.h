#pragma once
class Point2d
{
private:
    int x;
    int y;

public:
    Point2d(int x, int y);
    Point2d();
    void setLocation(int x, int y);
    void setLocation(Point2d p);
    int getX();
    int getY();
    Point2d operator-(Point2d other);
    Point2d operator+(Point2d other);
    Point2d operator*(int scalar);
    bool operator==(Point2d other);
    float getAngle();
};