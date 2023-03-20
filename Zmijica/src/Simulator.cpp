#include "Simulator.h"
#include "SnakeHuman.h"
#include "SnakeHamilton.h"
#include "Config.h"
#include <cstdlib>
#include <algorithm>
#include "SnakeAINN.h"
#include "SnakeAICGP.h"
#include "SnakeAIGP.h"
#include "Random.h"


void Simulator::addSnake(std::string type, Point2d startPosition)
{
    SnakeBase* toAdd = nullptr;
    if (type.at(0) != 'H')
    {
        std::shared_ptr<SnakeBase> toAdd;
        try
        {
            toAdd.reset(new SnakeAINN(startPosition, liveSnakes.size(), type));
        }
        catch (std::exception e)
        {
            std::string message = e.what();
            if (message.substr(0, 5) != "Path:")
                throw e;

            //spremljena zmija nije bila nn
            try
            {
                toAdd.reset(new SnakeAICGP(startPosition, liveSnakes.size(), type));
            }
            catch (std::exception e)
            {
                std::string message = e.what();
                if (message.substr(0, 5) != "Path:")
                    throw e;

                //spremljena zmija nije bila cgp
                toAdd.reset(new SnakeAIGP(startPosition, liveSnakes.size(), type));
            }
        }
        liveSnakes.push_back(toAdd);
    }
    else
    {
        if (type == "Human")
            liveSnakes.push_back(std::shared_ptr<SnakeBase>(new SnakeHuman(startPosition, liveSnakes.size())));
        else if (type == "Hamilton")
            liveSnakes.push_back(std::shared_ptr<SnakeBase>(new SnakeHamilton(startPosition, liveSnakes.size(), getMapSize())));
    }
}

Simulator::Simulator(unsigned int mapSize, std::vector<SnakeBase*> snakes)
{
    this->mapSize = mapSize;

    this->map.resize(mapSize, std::vector<int>(mapSize));
    this->hasApple = false; //na pocetku nema jabuke
    Point2d snakeStartPosition = Point2d(mapSize / 2, mapSize / 2);

    if (snakes.empty())
    {
        snakeStartPosition.setLocation(snakeStartPosition.getX(), Config::mapSize / Config::snakesPerSim / 2);

        for (int i = 0; i < Config::snakesPerSim; i++)
        {
            switch (i)
            {
            case(0):
                addSnake(Config::playModePlayer1, snakeStartPosition);
                break;
            case(1):
                addSnake(Config::playModePlayer2, snakeStartPosition);
                break;
            case(2):
                addSnake(Config::playModePlayer3, snakeStartPosition);
                break;
            case(3):
                addSnake(Config::playModePlayer4, snakeStartPosition);
                break;
            default:
                break;
            }

            snakeStartPosition.setLocation(snakeStartPosition.getX(), snakeStartPosition.getY() + Config::mapSize / Config::snakesPerSim);
        }
    }
    else
    {
        snakeStartPosition.setLocation(snakeStartPosition.getX(), Config::mapSize / snakes.size() / 2);

        for (int i = 0; i < snakes.size(); i++)
        {
            SnakeAINN* nnSnake = dynamic_cast<SnakeAINN*>(snakes.at(i));
            if (nnSnake)
                liveSnakes.push_back(std::shared_ptr<SnakeBase>(new SnakeAINN(snakeStartPosition, liveSnakes.size(), *nnSnake)));

            SnakeAICGP* cgpSnake = dynamic_cast<SnakeAICGP*>(snakes.at(i));
            if (cgpSnake)
                liveSnakes.push_back(std::shared_ptr<SnakeBase>(new SnakeAICGP(snakeStartPosition, liveSnakes.size(), *cgpSnake)));

            SnakeAIGP* gpSnake = dynamic_cast<SnakeAIGP*>(snakes.at(i));
            if (gpSnake)
                liveSnakes.push_back(std::shared_ptr<SnakeBase>(new SnakeAIGP(snakeStartPosition, liveSnakes.size(), *gpSnake)));


            snakeStartPosition.setLocation(snakeStartPosition.getX(), snakeStartPosition.getY() + Config::mapSize / snakes.size());
        }
    }

    // "crtanje" živih zmija u matricu
    for (int i = 0; i < liveSnakes.size(); i++) {
        int snakeIndex = i + 2; // indeks zmije za prikaz na mapi
        SnakeBase* currentSnake = &*liveSnakes[i];
        
        for (Point2d& cell : currentSnake->getSnakeCells()) {
            this->map[cell.getX()][cell.getY()] = snakeIndex;
        }
    }

}

unsigned int Simulator::getMapSize()
{
    return mapSize;
}

std::vector<std::vector<int>> Simulator::getMap()
{
    return this->map;
}

std::vector<std::shared_ptr<SnakeBase>>& Simulator::getLiveSnakes()
{
    return liveSnakes;
}

std::vector<std::shared_ptr<SnakeBase>>& Simulator::getDeadSnakes()
{
    return deadSnakes;
}

bool Simulator::step()
{   
    //generiranje random pozicije jabuke
    if (!this->hasApple) {
        int apple_x, apple_y;
        apple_x = Random::generateInt(0, mapSize - 1);
        apple_y = Random::generateInt(0, mapSize - 1);

        int fw_x, fw_y, bw_x, bw_y, counter = 1;
        fw_x = bw_x = apple_x;
        fw_y = bw_y = apple_y;
        while (this->map[apple_x][apple_y] != 0 && counter != mapSize * mapSize) {
            if (bw_y - 1 < 0) {                         //skoci na kraj proslog reda
                --bw_x;
                bw_y = mapSize;
            }
            if (fw_y + 1 == mapSize) {     //skoci na pocetak iduceg reda
                ++fw_x;
                fw_y = -1;
            }

            --bw_y;
            ++fw_y;

            if (bw_x >= 0 && bw_y >= 0 && bw_x <= mapSize-1 && bw_y <= mapSize-1) {
                if (this->map[bw_x][bw_y] == 0) {
                    apple_x = bw_x;
                    apple_y = bw_y;
                }
                counter++;
            }
            if (fw_x >= 0 && fw_y >= 0 && fw_x <= mapSize-1 && fw_y <= mapSize-1) {
                if (this->map[fw_x][fw_y] == 0) {
                    apple_x = fw_x;
                    apple_y = fw_y;
                }
                counter++;
            }
        }

        if (this->map[apple_x][apple_y] == 0) {
            this->map[apple_x][apple_y] = 1;    //pozicija jabuke
            this->hasApple = true;
        }
    }

    std::vector<int> tempDeadSnakes;
    
    for (int i = 0; i < liveSnakes.size(); i++)
    {
        std::shared_ptr<SnakeBase> currentSnake = liveSnakes[i];
        int snakeIndex = currentSnake->getIndex() + 2; // indeks zmije za prikaz na mapi

        Action result = currentSnake->step(map);
        std::list<Point2d> snakeCells = currentSnake->getSnakeCells();
        Point2d snakeHead = snakeCells.front();
        Point2d nextHeadPosition = Point2d();

        auto iterator = snakeCells.begin();
        std::advance(iterator, 1);
        Point2d secondCell = *iterator;

        // ovisno o trenutnom položaju zmijine glave i ćeliji prije glave, postavi iduću poziciju glave
        switch (result)
        {
        case(Action::LEFT):
            if (snakeHead.getX() == secondCell.getX() && snakeHead.getY() - 1 == secondCell.getY()) {
                nextHeadPosition.setLocation(Point2d(snakeHead.getX() - 1, snakeHead.getY()));
            }
            if (snakeHead.getX() == secondCell.getX() && snakeHead.getY() + 1 == secondCell.getY()) {
                nextHeadPosition.setLocation(Point2d(snakeHead.getX() + 1, snakeHead.getY()));
            }
            if (snakeHead.getX() - 1 == secondCell.getX() && snakeHead.getY() == secondCell.getY()) {
                nextHeadPosition.setLocation(Point2d(snakeHead.getX(), snakeHead.getY() + 1));
            }
            if (snakeHead.getX() + 1 == secondCell.getX() && snakeHead.getY() == secondCell.getY()) {
                nextHeadPosition.setLocation(Point2d(snakeHead.getX(), snakeHead.getY() - 1));
            }
            break;

        case(Action::RIGHT):
            if (snakeHead.getX() == secondCell.getX() && snakeHead.getY() - 1 == secondCell.getY()) {
                nextHeadPosition.setLocation(Point2d(snakeHead.getX() + 1, snakeHead.getY()));
            }
            if (snakeHead.getX() == secondCell.getX() && snakeHead.getY() + 1 == secondCell.getY()) {
                nextHeadPosition.setLocation(Point2d(snakeHead.getX() - 1, snakeHead.getY()));
            }
            if (snakeHead.getX() - 1 == secondCell.getX() && snakeHead.getY() == secondCell.getY()) {
                nextHeadPosition.setLocation(Point2d(snakeHead.getX(), snakeHead.getY() - 1));
            }
            if (snakeHead.getX() + 1 == secondCell.getX() && snakeHead.getY() == secondCell.getY()) {
                nextHeadPosition.setLocation(Point2d(snakeHead.getX(), snakeHead.getY() + 1));
            }
            break;

        case(Action::STRAIGHT):
            if (snakeHead.getX() == secondCell.getX() && snakeHead.getY() - 1 == secondCell.getY()) {
                nextHeadPosition.setLocation(Point2d(snakeHead.getX(), snakeHead.getY() + 1));
            }
            if (snakeHead.getX() == secondCell.getX() && snakeHead.getY() + 1 == secondCell.getY()) {
                nextHeadPosition.setLocation(Point2d(snakeHead.getX(), snakeHead.getY() - 1));
            }
            if (snakeHead.getX() - 1 == secondCell.getX() && snakeHead.getY() == secondCell.getY()) {
                nextHeadPosition.setLocation(Point2d(snakeHead.getX() + 1, snakeHead.getY()));
            }
            if (snakeHead.getX() + 1 == secondCell.getX() && snakeHead.getY() == secondCell.getY()) {
                nextHeadPosition.setLocation(Point2d(snakeHead.getX() - 1, snakeHead.getY()));
            }
            break;

        default:
            break;
        }

        int hx = nextHeadPosition.getX();
        int hy = nextHeadPosition.getY();
        int atHead;
        if (!(0 <= hx && hx < mapSize) || !(hy >= 0 && hy < mapSize))
            atHead = -1;
        else
            atHead = map[hx][hy];

        if (SnakeAIBase* sn = dynamic_cast<SnakeAIBase*>(currentSnake.get()))
            if (sn->getStepsSinceLastApple() > Config::mapSize * Config::mapSize + 1)
                atHead = -1;

        switch (atHead)
        {
        case(0):        //čisto
        {
            Point2d snakeTail = snakeCells.back();
            this->map[snakeTail.getX()][snakeTail.getY()] = 0;  //očistimo polje di je bio rep zmiji
            this->map[nextHeadPosition.getX()][nextHeadPosition.getY()] = snakeIndex;   //nova pozicija glave u matrici
            currentSnake->pushFront(nextHeadPosition);
            currentSnake->popBack();
            break;
        }
		case(1):        //jabuka
			currentSnake->addScore();
            this->hasApple = false;
            this->map[nextHeadPosition.getX()][nextHeadPosition.getY()] = snakeIndex;
            currentSnake->pushFront(nextHeadPosition);
            break;
		default:        //ili zid ili druga zmija
            tempDeadSnakes.push_back(i);
			deadSnakes.push_back(currentSnake);
            for (int x = 0; x < mapSize; x++){     //brisanje nedavno preminule zmije s mape
                for (int y = 0; y < mapSize; y++){
                    if (this->map[x][y] == snakeIndex) this->map[x][y] = 0;
                };
            };
            break;
        }
    }

    std::sort(tempDeadSnakes.rbegin(), tempDeadSnakes.rend());
    for (int i = 0; i < tempDeadSnakes.size(); i++)
    {
        liveSnakes.erase(liveSnakes.begin() + tempDeadSnakes[i]);   // brisanje zmija iz živih
    }

    return liveSnakes.size() != 0;
}