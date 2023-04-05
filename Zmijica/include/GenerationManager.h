#pragma once

#include <vector>
#include <string>
#include "Simulator.h"
#include "SnakeAIBase.h"

class GenerationManager
{
private:
    std::shared_ptr<SnakeBase> challengeSnake;
    unsigned int genNumber;
    std::vector<std::shared_ptr<Simulator>> allSimulators;
    //std::shared_ptr<SnakeBase> challengeSnake;
    std::vector<SnakeAIBase*> getAllSnakesSorted();
    std::vector<const SnakeAIBase*> cullCutoff(unsigned int count);
    std::vector<const SnakeAIBase*> cullGeometric(unsigned int count);
    std::vector<const SnakeAIBase*> cull();
    void generateSnakes(std::vector<SnakeAIBase*>& newSnakes, std::vector<const SnakeAIBase*> seed);

public:
    GenerationManager();

    /// <returns>Pointer to first simulator with living snake. Nullptr if none.</returns>
    Simulator* step();

    void nextGeneration();
    unsigned int* getGenerationNumber();
};