#include "GenerationManager.h"
#include "Config.h"
#include "SnakeAINN.h"
#include "SnakeAICGP.h"
#include "SnakeAIGP.h"
#include "Random.h"
#include <algorithm>
#include <iostream>
#include "SnakeHamilton.h"

std::vector<SnakeAIBase*> GenerationManager::getAllSnakesSorted()
{
    std::vector<SnakeAIBase*> allSnakes;

    for (std::shared_ptr<Simulator> s : allSimulators)
    {
        std::vector<SnakeAIBase*> simSnakes;
        for (std::shared_ptr<SnakeBase> sn : s->getLiveSnakes())
        {
            simSnakes.push_back((SnakeAIBase*)sn.get());
        }
        for (std::shared_ptr<SnakeBase> sn : s->getDeadSnakes())
        {
            simSnakes.push_back((SnakeAIBase*)sn.get());
        }

        allSnakes.insert(allSnakes.begin(), simSnakes.begin(), simSnakes.end());
    }

    std::sort(allSnakes.begin(), allSnakes.end(), [](SnakeAIBase* s1, SnakeAIBase* s2) {
        /*if (s1->getScore() == s2->getScore())
            return s1->getDistanceToApple() < s2->getDistanceToApple();*/

        return s1->getScore() > s2->getScore();
        });

    return allSnakes;
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
    this->challengeSnake = std::shared_ptr<SnakeBase>(new SnakeHamilton(Point2d(), 0, Config::mapSize));
    //this->challengeSnake = new SnakeHamilton(Point2d(), 0, Config::mapSize);
    for (int i = 0; i < Config::populationSize; i++)
    {
        std::vector<SnakeBase*> simSnakes(2);
        //simSnakes.reserve(Config::snakesPerSim + 1);
        //SnakeBase* snake = this->challengeSnake;
        //simSnakes.push_back(snake);
        //simSnakes.push_back(new SnakeHamilton(Point2d(), 0, Config::mapSize));
        simSnakes[0] = this->challengeSnake.get(); //nije radilo - opet garbage value - nekad negdje izaðe iz scopea?

        for (int j = 0; j < Config::snakesPerSim; j++)
        {
            SnakeBase* s  = NULL;

            if (Config::AIModel == "NN")
                s = new SnakeAINN(Point2d(), NULL, Config::hiddenLayerDepth, Config::hiddenLayerWidth, Config::activationFunction);
            else if (Config::AIModel == "CGP")
                s = new SnakeAICGP(Point2d(), NULL, Config::numOfRows, Config::numOfCols, Config::numOfFunctions, Config::numOfFunctionArgs);
            else if (Config::AIModel == "GP")
                s = new SnakeAIGP(Point2d(), NULL, Config::maxDepth, Config::numOfFunctions);

            simSnakes[1] = s;
            //simSnakes.push_back(s);
        }

        std::shared_ptr<Simulator> sim = std::shared_ptr<Simulator>(new Simulator(Config::mapSize, simSnakes));
        
        allSimulators.push_back(sim);
        //for (SnakeBase* s : simSnakes)
        //    delete s;
    }
}

Simulator* GenerationManager:: step()
{
    Simulator* firstWithSnake = nullptr;

    for (int i = 0; i < allSimulators.size(); i++)
    {
        if (allSimulators.at(i)->step() && !firstWithSnake)
            firstWithSnake = allSimulators.at(i).get();
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

    allSimulators.clear();

    for (int i = 0; i < Config::populationSize; i++)
    {
        std::vector<SnakeBase*> simSnakes;
        for (int j = 0; j < Config::snakesPerSim; j++)
        {
            simSnakes.push_back(newSnakes.at(i * Config::snakesPerSim + j));
        }

        std::shared_ptr<Simulator> sim = std::shared_ptr<Simulator>(new Simulator(Config::mapSize, simSnakes));
        allSimulators.push_back(sim);
    }
    genNumber++;

    for (SnakeAIBase* sn : newSnakes)
        delete sn;
}

unsigned int* GenerationManager::getGenerationNumber()
{
    return &genNumber;
}
