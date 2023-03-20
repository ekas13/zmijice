#include <stdexcept>
#include "CGP.h"
#include "Random.h"
#include <algorithm>
#include <iterator>
#include <string>

#define MAX_FUNCTIONS 6


CGP::CGP(CGP* src) {
    this->CGPData = src->CGPData;
}

int CGP::getNumOfRows() {
    return this->CGPData.numOfRows;
}

int CGP::getNumOfCols() {
    return this->CGPData.numOfCols;
}

int CGP::getNumOfFunctionArgs() {
    return this->CGPData.numOfFunctionArgs;
}

int CGP::getNumOfFunctions() {
    return this->CGPData.numOfFunctions;
}

std::vector<int> CGP::getGenotype() {
    return this->CGPData.genotype;
}

void CGP::setGenotype(std::vector<int> genotype) {
    this->CGPData.genotype = genotype;
}


CGP::CGP(int numOfInputs, int numOfOutputs, int numOfRows, int numOfCols, int numOfFunctions, int numOfFunctionArgs) {
    if (numOfInputs < 1 || numOfOutputs < 1 || numOfRows < 1 || numOfCols < 1 || numOfFunctions < 1) {
        throw std::invalid_argument("one of the arguments was lower than 1");
    }
    if (numOfFunctionArgs < 2) {
        throw std::invalid_argument("function argument number can't be less than 2");
    }
    if (numOfFunctions > MAX_FUNCTIONS) {
        throw std::invalid_argument("invalid number of functions");
    }
    this->CGPData.numOfInputs = numOfInputs;
    this->CGPData.numOfOutputs = numOfOutputs;
    this->CGPData.numOfRows = numOfRows;
    this->CGPData.numOfCols = numOfCols;
    this->CGPData.numOfFunctions = numOfFunctions;
    this->CGPData.numOfFunctionArgs = numOfFunctionArgs;
    this->CGPData.genotype = CGP::generateGenotype();
}


float CGP::applyFunction(int function, std::vector<float> inputs, std::vector<int> arguments) {
    std::vector<float> currInputs;      //inputi u funkciju
    float result = 0;                  

    for (int i = 0; i < arguments.size(); i++) {    //arguments su indeksi inputa u prevOutputs
        float newInput = inputs.at(arguments.at(i));
        currInputs.push_back(newInput);
    }

    switch (function) {
        case 0: //min
            result = *std::min_element(std::begin(currInputs), std::end(currInputs));;
            break;

        case 1: //max
            result = *std::max_element(std::begin(currInputs), std::end(currInputs));;
            break;

        case 2: //zbrajanje
            for (int i = 0; i < currInputs.size(); i++) {
                result += currInputs.at(i);
            }
            break;

        case 3: //oduzimanje
            result = currInputs.at(0);
            for (int i = 1; i < currInputs.size(); i++) {
                result -= currInputs.at(i);
            }
            break;

        case 4: //mnozenje
            result = 1;
            for (int i = 1; i < currInputs.size(); i++) {
                result *= currInputs.at(i);
            }
            break;

        case 5: //dijeljenje
            result = currInputs.at(0);
            for (int i = 1; i < currInputs.size(); i++) {
                if (currInputs.at(i) != 0) result /= currInputs.at(i);
            }
            break;
    }
    return result;
}


std::vector<int> CGP::generateGenotype() {
    std::vector<int> genotype;  //oblik -> arg arg... FUN arg arg... FUN ...

    for (int i = 0; i < CGPData.numOfCols; i++) {
        int argOut = i * CGPData.numOfRows + CGPData.numOfInputs;

        for (int j = 0; j < CGPData.numOfRows; j++) {
            for (int z = 0; z < CGPData.numOfFunctionArgs; z++) {
                genotype.push_back(Random::generateInt(0, argOut - 1));
            }
            genotype.push_back(Random::generateInt(0, CGPData.numOfFunctions - 1));
        }
    }

    int currArgOut = CGPData.numOfInputs + CGPData.numOfRows * CGPData.numOfCols;
    for (int i = 0; i < CGPData.numOfOutputs; i++) {
        genotype.push_back(Random::generateInt(0, currArgOut - 1));
    }

    return genotype;
}

std::vector<float> CGP::calculateOutputs(std::vector<float> inputs) {
    std::vector<float> results(CGPData.numOfOutputs, 0.0);
    std::vector<float> outputs = inputs;    //inicijalni inputi

    for (int i = 0; i < CGPData.numOfCols; i++) {
        std::vector<float> currColOutputs = outputs;    //inputi za iduci column ce biti outputi ovog columna

        for (int j = 0; j < CGPData.numOfRows; j++) {
            int currFunctionPosition = (i * CGPData.numOfRows + j) * (CGPData.numOfFunctionArgs + 1) + CGPData.numOfFunctionArgs;  //kalkulira poziciju trenutne funkcije
            int currFunction = CGPData.genotype.at(currFunctionPosition);
            std::vector<int> currArguments;

            for (int k = currFunctionPosition - CGPData.numOfFunctionArgs; k < currFunctionPosition; k++) {
                currArguments.push_back(CGPData.genotype.at(k));
            }

            outputs.push_back(applyFunction(currFunction, currColOutputs, currArguments));
        }
    }

    for (int i = 0; i < CGPData.numOfOutputs; i++) {
        int currPosition = CGPData.genotype.at(CGPData.genotype.size() - CGPData.numOfOutputs + i);
        results[i] = outputs.at(currPosition);
    }

    return results;
}


void CGP::mutate(float chanceOfMutation) {
    for (int index = 0; index < CGPData.genotype.size(); index++) {
        int newRandomValue = CGPData.genotype.at(index);

        if (fabs(Random::generateFloat()) < chanceOfMutation) {
            if (index < CGPData.genotype.size() - CGPData.numOfOutputs) {

                //ako je ta pozicija funkcija
                if (index % (CGPData.numOfFunctionArgs + 1) == CGPData.numOfFunctionArgs) {
                    newRandomValue = Random::generateInt(0, CGPData.numOfFunctions - 1);
                }
                //ako je ta pozicija argument
                else {
                    int currCol = index / (CGPData.numOfRows * (CGPData.numOfFunctionArgs + 1));
                    int maxOutput = currCol * CGPData.numOfRows + CGPData.numOfInputs;
                    newRandomValue = Random::generateInt(0, maxOutput - 1);
                }
            }
            //ako je ta pozicija output
            else {
                newRandomValue = Random::generateInt(0, CGPData.numOfInputs + CGPData.numOfCols * CGPData.numOfRows - 1);
            }
        }
        CGPData.genotype[index] = newRandomValue;
    }
}


void CGP::cross(CGP* other, std::string typeOfCross) {

    if (typeOfCross == "disable")
        return;

    int breakingPoint = Random::generateInt(0, CGPData.genotype.size() - 1);       // random mjesto "pucanja" genotipova
    int initialModel = Random::generateInt(0, 1);
    std::vector<int> newGenotype;

    //lijeva strana od ovog modela, desna od other
    if (initialModel == 0) {
        for (int i = 0; i < breakingPoint; i++) {
            newGenotype.push_back(this->CGPData.genotype.at(i));
        }
        for (int i = breakingPoint; i < CGPData.genotype.size(); i++) {
            newGenotype.push_back(other->CGPData.genotype.at(i));
        }
    }
    //lijeva strana od other modela, desna od ovog
    else {
        for (int i = 0; i < breakingPoint; i++) {
            newGenotype.push_back(other->CGPData.genotype.at(i));
        }
        for (int i = breakingPoint; i < CGPData.genotype.size(); i++) {
            newGenotype.push_back(this->CGPData.genotype.at(i));
        }
    }
    this->CGPData.genotype = newGenotype;
}