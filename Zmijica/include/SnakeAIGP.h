#pragma once

#include <vector>
#include <string>
#include "SnakeBase.h"
#include "GP.h"
#include "SnakeAIBase.h"

class SnakeAIGP : public SnakeAIBase
{
private:
    GP* model;
    Action mapToAction(float output);
    void saveSnakeData();
    void recursiveTreeSave(GPNode* node, int i);
    GPNode* recursiveTreeLoad(int i);
    
public:
    SnakeAIGP(Point2d headPosition, int index, int maxDepth, int numOfFunction);
    SnakeAIGP(Point2d headPosition, int index, const SnakeAIGP& oldSnake);
    SnakeAIGP(Point2d headPosition, int index, std::string path);
    void cross(SnakeAIBase& other, std::string typeOfCross) ;
    void mutate(float chanceOfMutation) ;
    Action step(std::vector<std::vector<int>> map) ;
    virtual ~SnakeAIGP();
};