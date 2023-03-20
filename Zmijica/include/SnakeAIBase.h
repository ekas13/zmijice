#pragma once
#include "SnakeBase.h"
#include "Saveable.h"
#include <vector>
#include <string>

class SnakeAIBase: public SnakeBase, public Saveable
{
private:
    std::string visionType;
    unsigned int numInputs;
    int stepsSinceLastApple;
    float distanceToApple;
    //vraca array vrijednosti ovisno o polozaju zmijine glave naspram ostatku tijela
    //te vrijednosti sluze za prolazak po svim smjerovima 
    std::vector<std::vector<int>> calculateDirections();

    //za zadanu mapu, smjer i poziciju zmijine glave, vraca udaljenost do prvog objekta i njegov tip
    std::vector<float> calculateNearestDistance(std::vector<int> direction, std::vector<std::vector<int>> map);
    std::vector<float> getInputsRaycast7(std::vector<std::vector<int>> map);

    void saveData() override;

protected:
    //za zadanu mapu, vrati za svih 7 smjerova zmije vector tipa [udaljenost1, tipObjekta1, ... udaljenost7, tipObjekta7] 
    std::vector<float> getInputs(std::vector<std::vector<int>> map);
    unsigned int getNumberOfInputs() const;
    virtual void saveSnakeData() = 0;

public:
    SnakeAIBase(Point2d headPosition, int index);
    SnakeAIBase(Point2d headPosition, int index, std::string path);
    void setVisionType(std::string type);
    virtual ~SnakeAIBase();
    virtual void cross(SnakeAIBase& other, std::string typeOfCross) = 0;
    virtual void mutate(float chanceOfMutation) = 0;
    virtual Action step(std::vector<std::vector<int>> map) override;
    virtual void addScore() override;
    int getStepsSinceLastApple();
    int getDistanceToApple();
};