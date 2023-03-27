#include "TeamSimulator.h"
#include "Config.h"
#include <cstdlib>
#include <algorithm>
#include "Random.h"



TeamSimulator::TeamSimulator(unsigned int mapSize, std::vector<Team*> teams)
{
    this->mapSize = mapSize;

    this->map.resize(mapSize, std::vector<int>(mapSize));
    this->hasApple = false; //na pocetku nema jabuke
    Point2d snakeStartPosition = Point2d(mapSize / 2, mapSize / 2);

    this->teams = teams;

    snakeStartPosition.setLocation(snakeStartPosition.getX(), Config::mapSize / (teams.size()*2) / 2);
    //trebat ce kopirati team-ove iz gen managera?

    for (int i = 0; i < teams.size(); i++)
    {
        SnakeBase* snake = new SnakeBase(snakeStartPosition, liveSnakes.size());
        liveSnakes.push_back(snake);
        teams[i]->addSnake(snake);
        snakeStartPosition.setLocation(snakeStartPosition.getX(), snakeStartPosition.getY() + Config::mapSize / (teams.size() * 2));
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

unsigned int TeamSimulator::getMapSize()
{
    return mapSize;
}

std::vector<std::vector<int>> TeamSimulator::getMap()
{
    return this->map;
}

std::vector<Team*> TeamSimulator::getTeams()
{
    return this->teams;
}

bool TeamSimulator::step()
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

            if (bw_x >= 0 && bw_y >= 0 && bw_x <= mapSize - 1 && bw_y <= mapSize - 1) {
                if (this->map[bw_x][bw_y] == 0) {
                    apple_x = bw_x;
                    apple_y = bw_y;
                }
                counter++;
            }
            if (fw_x >= 0 && fw_y >= 0 && fw_x <= mapSize - 1 && fw_y <= mapSize - 1) {
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
        std::vector<Action> steps;
        if (i == 0)
            steps = teams[0]->step(map);
        else if (i == 2)
            steps = teams[1]->step(map);

        SnakeBase* currentSnake = liveSnakes[i];
        int snakeIndex = currentSnake->getIndex() + 2; // indeks zmije za prikaz na mapi

        Action result = steps[i%2];
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

        if (SnakeAIBase* sn = dynamic_cast<SnakeAIBase*>(currentSnake))
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
            
            currentSnake->addScore(); // ne treba 

        
            teams[i%2]->addTeamScore();
         

            this->hasApple = false;
            this->map[nextHeadPosition.getX()][nextHeadPosition.getY()] = snakeIndex;
            currentSnake->pushFront(nextHeadPosition);
            break;
        default:        //ili zid ili druga zmija
            if ((atHead >= 2 && atHead <= 5) && atHead != currentSnake->getIndex())   // ako je index neke zmije, a to nije njezin (nije se zabila sama u sebe)
            {
                teams[i % 2]->setTeamScore(teams[i % 2]->getTeamScore() - (int)(0.3 * currentSnake->getScore()));
                // ili samo kad izracunavam getScore napravit to po njegovim snakes-ima - to mi se cak vise svida "vise multi-agent"
                currentSnake->setScore((int)(0.7 * currentSnake->getScore()));
                

            }

            tempDeadSnakes.push_back(i);
            //deadSnakes.push_back(currentSnake);
            for (int x = 0; x < mapSize; x++) {     //brisanje nedavno preminule zmije s mape
                for (int y = 0; y < mapSize; y++) {
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
