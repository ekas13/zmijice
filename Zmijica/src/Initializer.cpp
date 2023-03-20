#include "Initializer.h"
#include "Graphics.h"
#include "Simulator.h"
#include "SnakeHuman.h"
#include "Direction.h"
#include "Config.h"
#include "GenerationManager.h"
#include <fstream>

void Initializer::saveSimSnake(Simulator* sim)
{
    std::vector<std::shared_ptr<SnakeBase>> snakes = sim->getLiveSnakes();
    if (snakes.empty())
        snakes = sim->getDeadSnakes();

    Saveable* toSave = dynamic_cast<Saveable*>(snakes.at(0).get());
    if (!toSave)
        throw std::invalid_argument("Simulator contains non saveable snakes.");

    int index = 0;
    std::ifstream check;
    do
    {
        index++;
        if (check.is_open())
            check.close();

        std::string path = "./saves/snake_" + std::to_string(index) + ".save";
        check.open(path);
    } while (check.good());
    if (check.is_open())
        check.close();

    toSave->save("./saves/snake_" + std::to_string(index) + ".save");
}

void Initializer::initializeLearn()
{
    GenerationManager manager;
    Graphics g(1280, 720, "Zmijica learning");

    unsigned int speed = 1;
    unsigned int stepSpeed;
    unsigned int graphicSpeed;

    g.setGameSpeedTracker(&speed);
    g.setGenerationTracker(manager.getGenerationNumber());

    Simulator* currentlyDisplayed = nullptr;

    while (g.isOpen())
    {
        sf::Event event;
        while (g.pollEvent(event))
        {
            if (event.type == sf::Event::KeyPressed)
            {
                if (event.key.code == sf::Keyboard::X)
                    speed *= 2;
                if (event.key.code == sf::Keyboard::Y && speed > 1)
                    speed /= 2;

                if (event.key.code == sf::Keyboard::S && currentlyDisplayed)
                    saveSimSnake(currentlyDisplayed);
            }
        }

        if (speed <= 8)
        {
            graphicSpeed = 8 / speed;
            stepSpeed = 1;
        }
        else
        {
            stepSpeed = speed / 8;
            graphicSpeed = 1;
        }

        Simulator* firstLive = nullptr;
        for (int i = 0; i < stepSpeed; i++)
        {
            firstLive = manager.step();
            if (!firstLive)
                manager.nextGeneration();
        }
        //relationship between step and draw determines game speed
        for (int i = 0; i < graphicSpeed; i++)
        {
            if (!g.draw() && firstLive)
            {
                g.setSimulator(firstLive);
                currentlyDisplayed = firstLive;
            }
        }
    }
}

void Initializer::initializePlay()
{
    Graphics g(1280, 720, "Zmijica");

    Simulator* sim = new Simulator(Config::mapSize);
    g.setSimulator(sim);

    bool live = true;
    int stepCount = 1;
    int graphicCount = 8 / Config::playSpeed;
    if (graphicCount == 0)
    {
        graphicCount = 1;
        stepCount = Config::playSpeed / 8;
    }

    while (g.isOpen())
    {
        sf::Event event;
        while (g.pollEvent(event))
        {
            if (event.type == sf::Event::KeyPressed)
            {
                if (event.key.code == sf::Keyboard::Up)
                    SnakeHuman::pushUserArrowInput(U);
                if (event.key.code == sf::Keyboard::Down)
                    SnakeHuman::pushUserArrowInput(D);
                if (event.key.code == sf::Keyboard::Left)
                    SnakeHuman::pushUserArrowInput(L);
                if (event.key.code == sf::Keyboard::Right)
                    SnakeHuman::pushUserArrowInput(R);

                if (event.key.code == sf::Keyboard::R /*&& !live*/)
                {
                    delete sim;
                    sim = new Simulator(Config::mapSize);
                    g.setSimulator(sim);
                    SnakeHuman::pushUserArrowInput(D);
                }
            }
        }

        for (int i = 0; i < stepCount; i++)
            live = sim->step();

        for (int i = 0; i < graphicCount; i++)
            g.draw(true);
    }

    delete sim;
}