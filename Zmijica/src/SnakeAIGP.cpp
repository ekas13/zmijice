#include "SnakeAIGP.h"
#include "Config.h"
#include <iostream>

Action SnakeAIGP::mapToAction(float output)
{
    float outputBound = 0.5;
    
    if (output < -outputBound)
        return LEFT;

    else if (output > outputBound)
        return RIGHT;

    else
        return STRAIGHT;
}

void SnakeAIGP::saveSnakeData()
{
    // ovdje treba spremiti sve relevantne varijable (GPData)
    // maxDepth, i druge varijable se moze spremiti pod tim istim nazivom, samo u GP treba dodati gettere
    // stablo je binarno pa ga se moze spremiti na sljedeci nacin
    // node_1 = korijen stabla
    // node_i = i-ti cvor
    // node_2*i = lijevo dijete i-tog cvora
    // node_2*i + 1 = desno dijete i-tog cvora

    //naziv klase
    save("class", "SnakeAIGP");

    //clanske varijable
    save("maxDepth", model->getMaxDepth());
    save("numOfFunctions", model->getNumOfFunctions());
    
    //spremanje stabla
    recursiveTreeSave(model->getRoot(), 1);

}

void SnakeAIGP::recursiveTreeSave(GPNode* node, int i) {

    std::string name = "node_" + std::to_string(i);
    save(name, node->getKey_value());

    if (node->getLeft()) recursiveTreeSave(node->getLeft(), 2 * i);
    if (node->getRight()) recursiveTreeSave(node->getRight(), 2 * i + 1);
}

SnakeAIGP::SnakeAIGP(Point2d headPosition, int index, int maxDepth, int numOfFunctions) : SnakeAIBase(headPosition, index)
{
    model = new GP(maxDepth, numOfFunctions, getNumberOfInputs());
}

SnakeAIGP::SnakeAIGP(Point2d headPosition, int index, const SnakeAIGP& oldSnake) : SnakeAIBase(headPosition, index)
{
    model = new GP(oldSnake.model);
}

SnakeAIGP::SnakeAIGP(Point2d headPosition, int index, std::string path) : SnakeAIBase(headPosition, index, path)
{
    // treba ucitati gpData iz pomocu load metoda
    // stablo ce se trebati ucitati iz niza varijabli na isti nacin na koji su spremljene u saveSnakeData
    // treba provjeriti ako node_i ne postoji onda se taj pointer postavlja na nullptr
    // ako ne postoji neka varijabla (node_i) metoda load ce baciti invalid argument pa treba koristiti try catch

    std::string className = "";
    load("class", className);
    if (className != "SnakeAIGP")
        throw std::invalid_argument("Path: " + path + " is not path to save file of SnakeAIGP class");

    int maxDepth = 0;
    int numOfFunctions = 0;
    load("maxDepth", maxDepth);
    load("numOfFunctions", numOfFunctions);

    model = new GP(maxDepth, numOfFunctions, getNumberOfInputs());

    model->setRoot(recursiveTreeLoad(1));
    //model->GPData.root = recursiveTreeLoad(1);
}

GPNode* SnakeAIGP::recursiveTreeLoad(int i) {

    int value;
    try {
        std::string name = "node_" + std::to_string(i);
        load(name, value);
    }
    catch (const std::invalid_argument& ia){
        return nullptr;
    }
    GPNode* new_node = new GPNode();
    new_node->setKey_value(value);
    new_node->setLeft(recursiveTreeLoad(2 * i));
    new_node->setRight(recursiveTreeLoad(2 * i + 1));
    return new_node;
}

Action SnakeAIGP::step(std::vector<std::vector<int>> map)
{
    SnakeAIBase::step(map);

    std::vector<float> inputs = getInputs(map);
    float output = model->calculateOutput(inputs).at(0);

    return mapToAction(output);
}

void SnakeAIGP::cross(SnakeAIBase& other, std::string typeOfCross)
{
    SnakeAIGP* oth = dynamic_cast<SnakeAIGP*>(&other);

    model->cross(oth->model, typeOfCross);
}

void SnakeAIGP::mutate(float chanceOfMutation)
{
    model->mutate(chanceOfMutation);
}

SnakeAIGP::~SnakeAIGP()
{
    delete model;
}