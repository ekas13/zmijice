#include "GenerationManager.h"
#include "Config.h"
#include "SnakeAINN.h"
#include "SnakeAICGP.h"
#include "SnakeAIGP.h"
#include "Random.h"
#include <algorithm>
#include <iostream>
#include <function>

std::vector<SnakeAIBase*> GenerationManager::getAllSnakesSorted()
{
    std::vector<SnakeAIBase*> allSnakes;

    for (Simulator s : allSimulators)
    {
        std::vector<SnakeAIBase*> simSnakes;
        for (std::shared_ptr<SnakeBase> sn : s.getLiveSnakes())
        {
            simSnakes.push_back((SnakeAIBase*)sn.get());
        }
        for (std::shared_ptr<SnakeBase> sn : s.getDeadSnakes())
        {
            simSnakes.push_back((SnakeAIBase*)sn.get());
        }

        allSnakes.insert(allSnakes.begin(), simSnakes.begin(), simSnakes.end());
    }

    adjustSnakeScores(allSnakes);

    std::sort(allSnakes.begin(), allSnakes.end(), [](SnakeAIBase* s1, SnakeAIBase* s2) {
        /*if (s1->getScore() == s2->getScore())
            return s1->getDistanceToApple() < s2->getDistanceToApple();*/

        return s1->getScore() > s2->getScore();
        });

    return allSnakes;
}

void GenerationManager::adjustSnakeScores(std::vector<SnakeAIBase*>& allSnakes)
{
    if (Config::learningType == "SARL")
        return;
    else if (Config::learningType == "MARL_cooperative")
    {
        
    }
    else if (Config::learningType == "MARL_competitive")
    {

    }
    else if (Config::learningType == "MARL_mixed")
    {

    }

}

std::vector<const SnakeAIBase*> GenerationManager::cullCutoff(unsigned int count)
{
    std::vector<SnakeAIBase*> allSnakes = getAllSnakesSorted();
    std::vector<const SnakeAIBase*> ret;

    for (int i = 0; i < count && i < allSnakes.size(); i++)
        ret.push_back(allSnakes.at(i));

    return ret;
}

std::vector<const SnakeAIBase*> GenerationManager::cullGeometric(unsigned int count)
{
    std::vector<SnakeAIBase*> allSnakes = getAllSnakesSorted();
    std::vector<const SnakeAIBase*> ret;

    std::vector<int> indexes;
    float prob = 1.0 / count;
    int maxTries = Config::populationSize;
    do
    {
        int i = Random::generateIntGeometric(prob);
        if (std::find(indexes.begin(), indexes.end(), i) == indexes.end() && i < allSnakes.size())
            indexes.push_back(i);

        maxTries--;
    } while (indexes.size() < count && maxTries > 0);

    while (indexes.size() <  count)
    {
        int i = Random::generateInt(0, allSnakes.size() - 1);
        if (std::find(indexes.begin(), indexes.end(), i) == indexes.end())
            indexes.push_back(i);
    }

    for (int i : indexes)
        ret.push_back(allSnakes.at(i));

    return ret;
}

std::vector<const SnakeAIBase*> GenerationManager::cull()
{
    if (Config::selectionType == "Cutoff")
        return cullCutoff(Config::selectionSize);
    else if (Config::selectionType == "Geometric")
        return cullGeometric(Config::selectionSize);

    throw std::exception("Selection type is invalid.");
}

void GenerationManager::generateSnakes(std::vector<SnakeAIBase*>& newSnakes, std::vector<const SnakeAIBase*> seed)
{
    //zmije iz seeda pripadaju simulatoru i on ce se pobrinuti za njihovo unistavanje
    //generation manager se mora pobrinuti za unistavanje newSnakes
    unsigned int seedSize = seed.size();

    while (newSnakes.size() < Config::populationSize * Config::snakesPerSim)
    {
        //odabiremo izvorisnu zmiju (jedna od najboljih iz prosle generacije)
        const SnakeAIBase* src = seed.at(Random::generateInt(0, seedSize - 1));

        const SnakeAINN* nn = dynamic_cast<const SnakeAINN*>(src);
        const SnakeAICGP* cgp;
        const SnakeAIGP* gp;
        if (nn)
        {
            newSnakes.push_back(new SnakeAINN(Point2d(), NULL, *nn));
        }
        else
        {
            cgp = dynamic_cast<const SnakeAICGP*>(src);
            if (cgp)
            {
                newSnakes.push_back(new SnakeAICGP(Point2d(), NULL, *cgp));
            }
            else
            {
                const SnakeAIGP* gp = dynamic_cast<const SnakeAIGP*>(src);
                if (gp)
                    newSnakes.push_back(new SnakeAIGP(Point2d(), NULL, *gp));
            }
        }
    }
}

GenerationManager::GenerationManager()
{
    genNumber = 1;
    for (int i = 0; i < Config::populationSize; i++)
    {
        std::vector<SnakeBase*> simSnakes;
        for (int j = 0; j < Config::snakesPerSim; j++)
        {
            SnakeBase* s;

            if (Config::AIModel == "NN")
                s = new SnakeAINN(Point2d(), NULL, Config::hiddenLayerDepth, Config::hiddenLayerWidth, Config::activationFunction);
            else if (Config::AIModel == "CGP")
                s = new SnakeAICGP(Point2d(), NULL, Config::numOfRows, Config::numOfCols, Config::numOfFunctions, Config::numOfFunctionArgs);
            else if (Config::AIModel == "GP")
                s = new SnakeAIGP(Point2d(), NULL, Config::maxDepth, Config::numOfFunctions);

            simSnakes.push_back(s);
        }

        Simulator sim(Config::mapSize, simSnakes);

        for (SnakeBase* s : simSnakes)
            delete s;

        allSimulators.push_back(sim);
    }
}

Simulator* GenerationManager:: step()
{
    Simulator* firstWithSnake = nullptr;

    for (int i = 0; i < allSimulators.size(); i++)
    {
        if (allSimulators.at(i).step() && !firstWithSnake)
            firstWithSnake = &allSimulators.at(i);
    }

    return firstWithSnake;
}

void GenerationManager::nextGeneration()
{
    std::vector<const SnakeAIBase*> seed = cull();
    std::vector<SnakeAIBase*> newSnakes;
    generateSnakes(newSnakes, seed);

    for (SnakeAIBase* sn : newSnakes)
        sn->cross(*(newSnakes.at(Random::generateInt(0, newSnakes.size() - 1))), Config::crossType);

    for (int i = 0; i < newSnakes.size(); i++)
        newSnakes.at(i)->mutate(Config::mutationChance);

    std::vector<Simulator> newSims;
    for (int i = 0; i < Config::populationSize; i++)
    {
        std::vector<SnakeBase*> simSnakes;
        for (int j = 0; j < Config::snakesPerSim; j++)
        {
            simSnakes.push_back(newSnakes.at(i * Config::snakesPerSim + j));
        }

        Simulator sim(Config::mapSize, simSnakes);
        newSims.push_back(sim);
    }

    allSimulators = newSims;
    genNumber++;

    for (SnakeAIBase* sn : newSnakes)
        delete sn;
}

unsigned int* GenerationManager::getGenerationNumber()
{
    return &genNumber;
}
