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

void Team::addSnake(std::shared_ptr<SnakeBase> snake)
{
    this->snakes.push_back(snake);
}

std::vector<Action> Team::step(std::vector<std::vector<int>> map)
{
    std::vector<float> teamInputs;
    std::vector<float> inputs;
    for (int i = 0; i < snakes.size(); i++)
    {
        std::shared_ptr<SnakeBase> snake = snakes[i];
        SnakeAIBase* currentSnake = dynamic_cast<SnakeAIBase*>(snake.get());
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

void Team::addDeadSnake(std::shared_ptr<SnakeBase> snake)
{
    this->deadSnakes.push_back(snake);
}

std::vector<std::shared_ptr<SnakeBase>>& Team::getLiveSnakes()
{
    return this->snakes;
}

std::vector<std::shared_ptr<SnakeBase>>& Team::getDeadSnakes()
{
    return this->deadSnakes;
}

std::vector<int> Team::allSnakeScores()
{
    std::vector<std::shared_ptr<SnakeBase>> allSnakes;
    int b = allSnakes.size();
    int al = this->snakes.size();
    int bl = this->deadSnakes.size();
    int alive = this->getLiveSnakes().size();
    int dead = this->getDeadSnakes().size();

    for (std::shared_ptr<SnakeBase> sn : this->getLiveSnakes()) {
        allSnakes.push_back(sn);
    }
    for (std::shared_ptr<SnakeBase> sn : this->getDeadSnakes()) {
        allSnakes.push_back(sn);
    }
    int a = allSnakes.size();
    int first = allSnakes[0]->getScore();
    int second = allSnakes[1]->getScore();
    return std::vector<int>{first,second};
}

int Team::getFirstSnakeScore()
{
    return this->allSnakeScores()[0];
}

int Team::getSecondSnakeScore()
{
    return this->allSnakeScores()[1];
}

void Team::removeSnakeAt(int index)
{
    //printf("remove: %d with %d living", index,this->getNoOfAliveSnakes());
    //this->snakes.erase(std::remove(snakes.begin(), snakes.end(), index), snakes.end());
    this->snakes.erase(snakes.begin() + (index % this->getNoOfAliveSnakes()));
}



//pazi bri�e� nemoj onda loopat po njima