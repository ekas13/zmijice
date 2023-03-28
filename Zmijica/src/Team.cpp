#include "Team.h"
#include "Config.h"
#include "SnakeBase.h"
#include <algorithm>
Team::Team()
{
    this->model = new GP(Config::maxDepth, Config::numOfFunctions, 28);
    this->teamScore = 0;

}

Team::Team(const Team& oldTeam)
{
    this->teamScore = 0;
    this->model = new GP(oldTeam.model);

}

void Team::addSnake(SnakeBase* snake) {
    this->snakes.push_back(snake);
}

std::vector<Action> Team::step(std::vector<std::vector<int>> map)
{
    std::vector<float> teamInputs;
    std::vector<float> inputs;
    for (int i = 0; i < snakes.size(); i++)
    {
        SnakeAIBase* currentSnake = dynamic_cast<SnakeAIBase*>(snakes[i]);
        inputs = currentSnake->getInputs(map);

        teamInputs.insert(teamInputs.end(), inputs.begin(), inputs.end());
    }
    float output = model->calculateOutput(teamInputs).at(0);
    std::vector<Action> steps = mapToActions(output);
    return steps;
    //agentsStep(steps);
}

std::vector<Action> Team::mapToActions(float output)
{
    float outputBound = 0.5;
    if (output < -1.75)
        return std::vector<Action>{LEFT, LEFT};
    else if (-1.75 < output < -1.25)
        return std::vector<Action>{ LEFT, RIGHT };
    else if (-1.25 < output < -0.75)
        return { RIGHT,LEFT };
    else if (-0.75 < output < -0.25)
        return std::vector<Action>{ RIGHT, STRAIGHT };
    else if (-0.25 < output < 0.25)
        return std::vector<Action>{ STRAIGHT, RIGHT };
    else if (0.25 < output < 0.75)
        return std::vector<Action>{ STRAIGHT, LEFT};
    else if (0.75 < output < 1.25)
        return std::vector<Action>{ LEFT, STRAIGHT};
    else if (1.25 < output < 1.75)
        return std::vector<Action>{ RIGHT, RIGHT };
    else if (1.75 < output)
        return std::vector<Action>{ STRAIGHT, STRAIGHT};
}

void Team::cross(Team& other, std::string typeOfCross)
{
    Team* oth = dynamic_cast<Team*>(&other);
    this->model->cross(oth->model, typeOfCross);
}

void Team::mutate(float chanceOfMutation)
{
    this->model->mutate(chanceOfMutation);
}

void Team::addTeamScore()
{
    int a = this->teamScore;
    this->teamScore++;
}

int Team::getNoOfAliveSnakes()
{
    return this->snakes.size();
}

int Team::getTeamScore()
{
    return this->teamScore;
}

void Team::setTeamScore(int score)
{
    this->teamScore = score;
}

void Team::removeSnakeAt(int index)
{
    //printf("remove: %d with %d living", index,this->getNoOfAliveSnakes());
    //this->snakes.erase(std::remove(snakes.begin(), snakes.end(), index), snakes.end());
    this->snakes.erase(snakes.begin() + (index % this->getNoOfAliveSnakes()));
}



//pazi brišeš nemoj onda loopat po njima