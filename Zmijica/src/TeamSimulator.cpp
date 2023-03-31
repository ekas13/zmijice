#include "TeamSimulator.h"
#include "Config.h"
#include <cstdlib>
#include <algorithm>
#include "Random.h"
#include "Snake.h"
#include <iostream>

TeamSimulator::TeamSimulator(unsigned int mapSize, std::vector<Team*> sentTeams, int generation)
{
    this->mapSize = mapSize;
    this->steps = 0;
    this->map.resize(mapSize, std::vector<int>(mapSize));
    this->hasApple = false; //na pocetku nema jabuke
    this->appleNo = 2*2;
    this->generation = generation;
    Point2d snakeStartPosition = Point2d(mapSize / 2, mapSize / 2);

    //this->teams; 

    snakeStartPosition.setLocation(snakeStartPosition.getX(), Config::mapSize / (sentTeams.size() * 2) / 2);
    //trebat ce kopirati team-ove iz gen managera?

    for (int i = 0; i < sentTeams.size(); i++) // no of teams
    {
        this->teams.push_back(std::shared_ptr<Team>(new Team(*sentTeams[i])));

        for (int j = 0; j < 2; j++) //no of snakes per team
        {

            SnakeBase* snake = new Snake(snakeStartPosition, liveSnakes.size());
            std::shared_ptr<SnakeBase> base = std::shared_ptr<SnakeBase>(snake);
            liveSnakes.push_back(base);
            //liveSnakes.push_back(std::shared_ptr<SnakeBase>(snake));
            this->teams[i]->addSnake(base);
            snakeStartPosition.setLocation(snakeStartPosition.getX(), snakeStartPosition.getY() + Config::mapSize / (sentTeams.size() * 2));
        }
    }

    // "crtanje" �ivih zmija u matricu
    for (int i = 0; i < liveSnakes.size(); i++) {
        int snakeIndex = i + 2; // indeks zmije za prikaz na mapi
        SnakeBase* currentSnake = &*liveSnakes[i];
        for (Point2d& cell : currentSnake->getSnakeCells()) {
            //printf("->%d %d ", cell.getX(), cell.getY());
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

std::vector<std::shared_ptr<Team>>& TeamSimulator::getTeams()
{
    return this->teams;
}

std::vector<std::shared_ptr<SnakeBase>>& TeamSimulator::getLiveSnakes()
{
    return liveSnakes;
}

bool TeamSimulator::step()
{
    this->steps++;
    //generiranje random pozicije jabuke
    if (this->hasApple < 4) {
        int apple_x, apple_y;
        apple_x = Random::generateInt(0, mapSize - 1);
        apple_y = Random::generateInt(0, mapSize - 1);

        int fw_x, fw_y, bw_x, bw_y, counter = 1;
        fw_x = bw_x = apple_x;
        fw_y = bw_y = apple_y;
        while (this->map[apple_x][apple_y] != 0 && counter != mapSize * mapSize) {
            if (bw_y - 1 < 0) {                         //sko�i na kraj proslog reda
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
            this->appleNo++;
        }
    }
    std::vector<int> tempDeadSnakes;

    for (int t = 0; t < this->teams.size(); t++)
    {
        std::vector<Action> steps;
        Team* currentTeam = &*(this->teams[t]);

        for (int i = 0; i < currentTeam->getLiveSnakes().size(); i++)
        {
            //int a = liveSnakes.size();
            if (i == 0) {
                steps = teams[t]->step(map);
            }
            
            std::shared_ptr<SnakeBase> base = currentTeam->getLiveSnakes()[i];
            SnakeBase* currentSnake = &*base;                                        //&*liveSnakes[i];
            int snakeIndex = currentSnake->getIndex() + 2;                           // indeks zmije za prikaz na mapi
            Action result = steps[i];                                                //i % 2

            currentSnake->step(map); // ugl samo pove�ava steps since last apple - za loop check - nemoj nigdje koristit povratnu vrijednost
            std::list<Point2d> snakeCells = currentSnake->getSnakeCells();
            Point2d snakeHead = snakeCells.front();
            Point2d nextHeadPosition = Point2d();

            auto iterator = snakeCells.begin();
            std::advance(iterator, 1);
            Point2d secondCell = *iterator;
            // ovisno o trenutnom polo�aju zmijine glave i �eliji prije glave, postavi idrugu poziciju glave
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
            if (this->steps > 800)
            {
                //teams[(i / 2)]->addToTeamScore(this->steps/2);
            }
            int hx = nextHeadPosition.getX();
            int hy = nextHeadPosition.getY();
            int atHead;

            if (!(0 <= hx && hx < mapSize) || !(hy >= 0 && hy < mapSize))
                atHead = -1;
            else
                atHead = map[hx][hy];

            Snake* aiBase = dynamic_cast<Snake*>(currentSnake);
            if (aiBase->getStepsSinceLastApple() > 100) {
                atHead = -3;   //- nema smisla mozda imat u kooperativnom jer ne znm jel loop ili izbjegava drugu zmiju pa vrluda
            }

            switch (atHead)
            {
            case(0):        //�isto
            {
                Point2d snakeTail = snakeCells.back();
                this->map[snakeTail.getX()][snakeTail.getY()] = 0;  //o�istimo polje di je bio rep zmiji
                this->map[nextHeadPosition.getX()][nextHeadPosition.getY()] = snakeIndex;   //nova pozicija glave u matrici
                currentSnake->pushFront(nextHeadPosition);
                currentSnake->popBack();
                break;
            }
            case(1):        //jabuka

                // fitness funkcija 
                /*int reward;
                if (liveSnakes.size() > 1 && this->teams[0]->getLiveSnakes()[std::abs(i - 1)]->getScore() > currentSnake->getScore())
                    reward = 10;
                else
                    reward = 3;
                teams[(i / 2)]->addToTeamScore(reward);*/
                currentSnake->addScore();

                this->hasApple = false;
                this->appleNo--;
                this->map[nextHeadPosition.getX()][nextHeadPosition.getY()] = snakeIndex;
                currentSnake->pushFront(nextHeadPosition);
                break;
            default:        //ili zid ili druga zmija
                if ((atHead >= 2 && atHead <= 5) && atHead != currentSnake->getIndex())   // ako je index neke zmije, a to nije njezin (nije se zabila sama u sebe)
                {
                    // teams[i / 2]->setTeamScore(teams[i / 2]->getTeamScore() - (int)(0.3 * currentSnake->getScore()));
                     // ili samo kad izracunavam getScore napravit to po njegovim snakes-ima - to mi se cak vise svida "vise multi-agent"
                     //currentSnake->setScore((int)(0.7 * currentSnake->getScore()));


                }
                if (atHead != -3) {
                    //teams[(i / 2)]->addToTeamScore(this->steps);//((int)(((float)(this->steps/2)/400.0)*100));
                }
                /*if (t == 0)
                    tempDeadSnakes.push_back(i);
                else if (t == 1)
                    tempDeadSnakes.push_back(liveSnakes.size()-1-i);*/

                currentTeam->addDeadSnake(base);                                           //this->teams[i / 2]
                currentTeam->removeSnakeAt(i);
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
            liveSnakes.erase(liveSnakes.begin() + tempDeadSnakes[i]);   // brisanje zmija iz �ivih
        }
    }
    
    if (this->steps > 800) {
        
        return false;
    }
    return liveSnakes.size() != 0;
}
