#include "SnakeAICGP.h"
#include "Config.h"
#include <stdexcept>

Action SnakeAICGP::mapToAction(float output) {
    Action action;

    if (output < -outputBound) {
        action = LEFT;
    }
    else if (output > outputBound) {
        action = RIGHT;
    }
    else {
        action = STRAIGHT;
    }

    return action;
}

void SnakeAICGP::saveSnakeData()
{
    save("class", "SnakeAICGP");
    save("numOfRows", model->getNumOfRows());
    save("numOfCols", model->getNumOfCols());
    save("numOfFunctions", model->getNumOfFunctions());
    save("numOfFunctionArgs", model->getNumOfFunctionArgs());
    save("genotype", model->getGenotype());
}

SnakeAICGP::SnakeAICGP(Point2d headPosition, int index, int numOfRows, int numOfCols, int numOfFunctions, int numOfFunctionArgs) : SnakeAIBase(headPosition, index) {
    model = new CGP(getNumberOfInputs(), 1, numOfRows, numOfCols, numOfFunctions, numOfFunctionArgs);
    outputBound = 0.5;
}

SnakeAICGP::SnakeAICGP(Point2d headPosition, int index, const SnakeAICGP& oldSnake) : SnakeAIBase(headPosition, index) {
    model = new CGP(oldSnake.model);
    outputBound = oldSnake.outputBound;
}

SnakeAICGP::SnakeAICGP(Point2d headPosition, int index, std::string path) : SnakeAIBase(headPosition, index, path)
{
    std::string className = "";
    load("class", className);
    if (className != "SnakeAICGP")
        throw std::invalid_argument("Path: " + path + " is not path to save file of SnakeAICGP class");

    int numOfRows = 0;
    int numOfCols = 0;
    int numOfFunctionArgs = 0;
    int numOfFunctions = 0;
    outputBound = 0.5;

    load("numOfRows", numOfRows);
    load("numOfCols", numOfCols);
    load("numOfFunctions", numOfFunctions);
    load("numOfFunctionArgs", numOfFunctionArgs);

    model = new CGP(getNumberOfInputs(), 1, numOfRows, numOfCols, numOfFunctions, numOfFunctionArgs);

    std::vector<int> genotype;
    load("genotype", genotype);
    if (genotype.size() != 0) model->setGenotype(genotype);
}

Action SnakeAICGP::step(std::vector<std::vector<int>> map) {
    SnakeAIBase::step(map);

    std::vector<float> inputs = getInputs(map);
    std::vector<float> outputs = model->calculateOutputs(inputs);

    return mapToAction(outputs.at(0));
}

void SnakeAICGP::cross(SnakeAIBase& other, std::string typeOfCross) {
    SnakeAICGP* oth = dynamic_cast<SnakeAICGP*>(&other);
    model->cross(oth->model, typeOfCross);
}

void SnakeAICGP::mutate(float chanceOfMutation) {
    model->mutate(chanceOfMutation);
}

SnakeAICGP::~SnakeAICGP() {
    delete model;
}