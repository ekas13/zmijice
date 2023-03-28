#pragma once

#include <vector>
#include <string>
#include "TeamSimulator.h"
#include <algorithm>

class TeamGenerationManager
{
private:
    std::vector<TeamSimulator> allSimulators;
    unsigned int genNumber;
    int teamNumber;

    std::vector<Team*> getAllTeamsSorted();
    std::vector<const Team*> cullCutoff(unsigned int count);
    //std::vector<const Team*> cullGeometric(unsigned int count);
    std::vector<const Team*> cull();
    void generateTeams(std::vector<Team*>& newTeams, std::vector<const Team*> seed);
    void adjustTeamScores(std::vector<Team*> teams);
    void printBest10teams(std::vector<Team*> teams);

public:
    TeamGenerationManager();

    /// <returns>Pointer to first simulator with living snake. Nullptr if none.</returns>
    TeamSimulator* step();

    void nextGeneration();
    unsigned int* getGenerationNumber();
};