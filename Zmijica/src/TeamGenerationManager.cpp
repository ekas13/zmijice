#include "TeamGenerationManager.h"
#include "Config.h"
#include "Random.h"



std::vector<Team*> TeamGenerationManager::getAllTeamsSorted()
{
    std::vector<Team*> teams;

    for (TeamSimulator s : allSimulators)
    {
        for (Team*t : s.getTeams()) 
        {
            teams.push_back(t);
        }

    }

    adjustTeamScores(teams);

    std::sort(teams.begin(), teams.end(), [](Team* t1, Team* t2) {

        return t1->getTeamScore() > t2->getTeamScore();
        });

    return teams;
}

void TeamGenerationManager::adjustTeamScores(std::vector<Team*> teams)
{
    if (Config::learningType == "SARL")
        return;
    else if (Config::learningType == "MARL_cooperative")
    {
        //int i = 0;
        //int n = allSnakesMap.size();
        //for (auto itr = allSnakesMap.begin(); itr != allSnakesMap.end(); ++itr, i++)
        //{
        //    TeamSimulator sim = allSimulators[(n - 1 - i) / Config::snakesPerSim];
        //    SnakeAIBase* snake = itr->first;
        //    if (itr->second == "dead")
        //        snake->setScore((int)(0.5 * snake->getScore()));
        //    else
        //        snake->setScore(snake->getScore());
        //}
        //for (int i = 0, n = allSnakes.size(); i < n; i++)
        //{
        //    Simulator sim = allSimulators[(n - 1 - i) / Config::snakesPerSim];
        //    SnakeAIBase* snake = allSnakes[i];
        //    float percentage = (float)(Config::snakesPerSim - sim.getNoOfDeadSnakes()) / Config::snakesPerSim;
        //    snake->setScore((Config::snakesPerSim - sim.getNoOfDeadSnakes()) * sim.getCumulativeSimulatorScore());
        //}
    }
    else if (Config::learningType == "MARL_competitive")
    {
        //for (int i = 0, n = allSnakes.size(); i < n; i++)
        //{
        //    Simulator sim = allSimulators[(n - 1 - i) / Config::snakesPerSim];
        //    SnakeAIBase* snake = allSnakes[i];
        //    snake->setScore(snake->getScore()-(Config::snakesPerSim - sim.getNoOfDeadSnakes()));
        //}

    }
    else if (Config::learningType == "MARL_mixed")
    {

    }

}

std::vector<const Team*> TeamGenerationManager::cullCutoff(unsigned int count)
{
    std::vector<Team*> teams = getAllTeamsSorted();
    std::vector<const Team*> ret;

    for (int i = 0; i < count && i < teams.size(); i++)
        ret.push_back(&*teams.at(i));

    return ret;
}

std::vector<const Team*> TeamGenerationManager::cull()
{
    if (Config::selectionType == "Cutoff")
        return cullCutoff(Config::selectionSize);
}

void TeamGenerationManager::generateTeams(std::vector<Team*>& newSnakes, std::vector<const Team*> seed)
{
    //zmije iz seeda pripadaju simulatoru i on ce se pobrinuti za njihovo unistavanje
    //generation manager se mora pobrinuti za unistavanje newSnakes
    unsigned int seedSize = seed.size();

    while (newSnakes.size() < Config::populationSize * Config::snakesPerSim)
    {
        //odabiremo izvorisnu zmiju (jedna od najboljih iz prosle generacije)
        const Team* src = seed.at(Random::generateInt(0, seedSize - 1));
        newSnakes.push_back(new Team(*dynamic_cast<const Team*>(src)));

    }
}

void TeamGenerationManager::nextGeneration()
{
    std::vector<const Team*> seed = cull();
    std::vector<Team*> newTeams;
    generateTeams(newTeams, seed);

    for (Team* t : newTeams)
        t->cross(*(newTeams.at(Random::generateInt(0, newTeams.size() - 1))), Config::crossType);

    for (int i = 0; i < newTeams.size(); i++)
        newTeams.at(i)->mutate(Config::mutationChance);

    std::vector<TeamSimulator> newSims;

    for (int i = 0; i < Config::populationSize; i++)
    {
        std::vector<Team*> simTeams;
        for (int j = 0; j < teamNumber; j++)
        {
            simTeams.push_back(newTeams.at(i * teamNumber + j));
        }
        TeamSimulator sim(Config::mapSize, simTeams);
        newSims.push_back(sim);

    }

    allSimulators = newSims;
    genNumber++;

    for (Team* sn : newTeams)
        delete sn;
}

TeamGenerationManager::TeamGenerationManager()
{
    genNumber = 1;
    this->teamNumber = 0;
    if (Config::learningType == "MARL_cooperative")
        this->teamNumber = 1;
    else if (Config::learningType == "MARL_mixed")
        this->teamNumber = 2;

    for (int i = 0; i < Config::populationSize; i++)
    {
        std::vector<Team*> simTeams;
        for (int j = 0; j < teamNumber; j++)
        {
            simTeams.push_back(new Team());
        }
        TeamSimulator sim(Config::mapSize,simTeams);
        allSimulators.push_back(sim);
    }
}

TeamSimulator* TeamGenerationManager::step()
{
    TeamSimulator* firstWithSnake = nullptr;

    for (int i = 0; i < allSimulators.size(); i++)
    {
        if (allSimulators.at(i).step() && !firstWithSnake)
            firstWithSnake = &allSimulators.at(i);
    }
    return firstWithSnake;
}

unsigned int* TeamGenerationManager::getGenerationNumber()
{
    return &genNumber;
}