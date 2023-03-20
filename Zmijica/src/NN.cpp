#include "NN.h"
#include "Config.h"
#include <random>
#include <stdexcept>
#include "Random.h"

#define MUTATION_STRENGTH 0.2

float* NN::findElement(unsigned int index)
{
    if (index >= numWeights)
        throw std::out_of_range("Index out of range.");

    for (std::vector<std::vector<float>>& layer : neuralNetwork)
    {
        for (std::vector<float>& node : layer)
        {
            if (index < node.size())
                return &node.at(index);
            index -= node.size();
        }
    }
    return nullptr;
}

unsigned int NN::getNumWeights()
{
    return numWeights;
}

std::vector<std::vector<float>> NN::createLayer(int numberOfElements, int widthOfLayer)
{
    std::vector<std::vector<float>> layer;
    for (int i = 0; i < widthOfLayer; i++)
    {
        std::vector<float> node;
        for (int j = 0; j < numberOfElements; j++)
        {
            float r = Random::generateFloat(-1, 1);
            node.push_back(r);
        }
        layer.push_back(node);
    }
    return layer;
}

NN::NN(NN* src)
{
    this->neuralNetwork = src->neuralNetwork;
    this->activationFunction = src->activationFunction;
    this->numWeights = src->numWeights;
}

NN::NN(int numberOfInputs, int numberOfOutputs, int hiddenLayerDepth, int hiddenLayerWidth, std::string activationFunctionName)
{
    if (numberOfInputs < 1 || numberOfOutputs < 1 || hiddenLayerDepth < 1 || hiddenLayerWidth < 1)
    {
        throw std::invalid_argument("one of the arguments was lower than 1");
    }

    if (activationFunctionName == "binary step")
    {
        activationFunction = [](float x)
        { return (x >= 0) ? 1.0f : 0.0f; };
    }
    else if (activationFunctionName == "linear")
    {
        activationFunction = [](float x)
        { return x; };
    }

    else if (activationFunctionName == "sigmoid")
    {
        activationFunction = [](float x)
        { return 1 / (1 + exp(-x)); };
    }
    else if (activationFunctionName == "tanh")
    {
        activationFunction = [](float x)
        { return (exp(x) - exp(-x)) / (exp(x) + exp(-x)); };
    }
    else if (activationFunctionName == "relu")
    {
        activationFunction = [](float x)
        { return (x <= 0) ? 0.0f : x; };
    }
    else
    {
        throw std::invalid_argument("not one of possible functions");
    }

    neuralNetwork.push_back(createLayer(numberOfInputs + 1, hiddenLayerWidth));

    for (int i = 0; i < hiddenLayerDepth; i++)
    {
        neuralNetwork.push_back(createLayer(hiddenLayerWidth + 1, hiddenLayerWidth));
    }
    neuralNetwork.push_back(createLayer(hiddenLayerWidth + 1, numberOfOutputs));

    numWeights = 0;
    for (std::vector<std::vector<float>> layer : neuralNetwork)
    {
        for (std::vector<float> node : layer)
        {
            numWeights += node.size();
        }
    }
}

std::vector<float> NN::calculateOutput(std::vector<float> inputs)
{
    std::vector<float> output;

    for (std::vector<float> node : neuralNetwork[0])
    {
        float result = 0.0;
        for (int i = 0, j = node.size() - 2; i < j; i++)
        {
            result += inputs[i] * node[i];
        }
        output.push_back(result + node[node.size() - 1]);
    }

    for (int i1 = 1, j1 = neuralNetwork.size() - 2; i1 < j1; i1++)
    {
        std::vector<float> elem;
        for (std::vector<float> node : neuralNetwork[i1])
        {
            float result = 0.0;
            for (int i = 0, j = node.size() - 2; i < j; i++)
            {
                result += activationFunction(output[i]) * node[i];
            }
            elem.push_back(result + node[node.size() - 1]);
        }
        output = elem;
    }

    std::vector<float> elem;
    for (std::vector<float> node : neuralNetwork[neuralNetwork.size() - 1])
    {
        float result = 0.0;
        for (int i = 0, j = node.size() - 2; i < j; i++)
        {
            result += output[i] * node[i];
        }
        elem.push_back(1/(1+exp(-(result + node[node.size() - 1]))));
    }
    output = elem;
    
    return output;
}

void NN::mutate(float chanceOfMutation)
{
    unsigned int weightsToModify = numWeights * chanceOfMutation;
    if (weightsToModify == 0)
        weightsToModify = 1;

    for (int i = 0; i < weightsToModify; i++)
    {
        int index = Random::generateInt(0, numWeights - 1);

        float* toModify = findElement(index);
        *toModify += Random::generateFloat(-MUTATION_STRENGTH, MUTATION_STRENGTH);
    }
}

void NN::cross(NN* other, std::string typeOfCross)
{

    if (typeOfCross == "disable")
        return;

    for (int i = 0, i2 = neuralNetwork.size(); i < i2; i++)
    {
        for (int j = 0, j2 = neuralNetwork[i].size(); j < j2; j++)
        {
            if (Random::generateFloat() < 0.02) {
                neuralNetwork[i][j] = other->neuralNetwork[i][j];
            }
        }
    }
}
