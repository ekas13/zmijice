#include "SnakeAINN.h"
#include <stdexcept>

#define LOWER_BOUND 0.33333
#define UPPER_BOUND 0.66667

Action SnakeAINN::mapToAction(float output)
{
    Action action;

    if (output < LOWER_BOUND) {
        action = LEFT;
    } else if (output > UPPER_BOUND) {
        action = RIGHT;
    } else {
        action = STRAIGHT;
    }

    return action;
}

void SnakeAINN::saveSnakeData()
{
    //ovo je funkcija koja omogucava spremanje zmije u file
    //dostupne su metode save(ime, varijabla) koje ucitavaju varijeble tipa int, float, string i vektore istih

    //spremamo naziv klase kako bismo mogli provjeriti ispravnost pri ucitavanju
    save("class", "SnakeAINN");

    //spremamo clanske varijable
    save("hiddenLayerDepth", hiddenLayerDepth);
    save("hiddenLayerWidth", hiddenLayerWidth);
    save("aFName", aFName);

    //spremamo tezine modela u formatu NN_wi (i je index)
    for (int i = 0; i < model->getNumWeights(); i++)
    {
        std::string name = "NN_w" + std::to_string(i);
        save(name, *model->findElement(i));
    }
}

SnakeAINN::SnakeAINN(Point2d headPosition, int index, int hiddenLayerDepth, int hiddenLayerWidth, std::string aFName) : SnakeAIBase(headPosition, index)
{
    model = new NN(getNumberOfInputs(), 1, hiddenLayerDepth, hiddenLayerWidth, aFName);
    this->hiddenLayerDepth = hiddenLayerDepth;
    this->hiddenLayerWidth = hiddenLayerWidth;
    this->aFName = aFName;
}

SnakeAINN::SnakeAINN(Point2d headPosition, int index, const SnakeAINN& oldSnake) : SnakeAIBase(headPosition, index) 
{
    model = new NN(oldSnake.model);
    this->hiddenLayerDepth = oldSnake.hiddenLayerDepth;
    this->hiddenLayerWidth = oldSnake.hiddenLayerWidth;
    this->aFName = oldSnake.aFName;
}

SnakeAINN::SnakeAINN(Point2d headPosition, int index, std::string path) : SnakeAIBase(headPosition, index, path)
{
    //ovo je konstruktor koji omogucava ucitavanje zmije iz filea
    //dostupne su metode load(ime, varijabla) koje ucitavaju varijeble tipa int, float, string i vektore istih

    //prvo ucitamo ime klase koja je spremljena da ne pokusamo ucitati krivi model
    std::string className = "";
    load("class", className);
    if (className != "SnakeAINN")
        throw std::invalid_argument("Path: " + path + " is not path to save file of SnakeAINN class");

    //sada ucitamo clanske varijable
    //nazivi pod kojim ih ucitavamo moraju odgovarati nazivima pod kojim smo ih spremili
    hiddenLayerDepth = 0;
    hiddenLayerWidth = 0;
    aFName = "";
    load("hiddenLayerDepth", hiddenLayerDepth);
    load("hiddenLayerWidth", hiddenLayerWidth);
    load("aFName", aFName);

    //number of inputs je ucitao SnakeAIBase a getNumberOfInputs() ga vraca
    model = new NN(getNumberOfInputs(), 1, hiddenLayerDepth, hiddenLayerWidth, aFName);
    //model je sada u ispravnoj konfiguraciji ali jos treba ucitati tezine (trenutno su tezine random)

    for (int i = 0; i < model->getNumWeights(); i++)
    {
        //ovo je odabrani format za spremanje imena tezina npr. 34. tezina je NN_w33
        std::string weightName = "NN_w" + std::to_string(i);
        //pronalazimo tu tezinu u mrezi
        float* weight = model->findElement(i);
        //ucitavamo je
        load(weightName, *weight);
    }
}

void SnakeAINN::cross(SnakeAIBase& other, std::string typeOfCross)
{
    SnakeAINN* oth = dynamic_cast<SnakeAINN*>(&other);

    model->cross(oth->model, typeOfCross);
}

void SnakeAINN::mutate(float chanceOfMutation)
{
    model->mutate(chanceOfMutation);
}

Action SnakeAINN::step(std::vector<std::vector<int>> map)
{
    SnakeAIBase::step(map);

    std::vector<float> inputs = getInputs(map);
    std::vector<float> outputs = model->calculateOutput(inputs);

    return mapToAction(outputs.at(0));
}

SnakeAINN::~SnakeAINN()
{
    delete model;
}