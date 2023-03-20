#include <vector>
#include <string>
#include <functional>

class NN
{
private:
    unsigned int numWeights;
    std::vector<std::vector<float>> createLayer(int numberOfElements, int widthOfLayer);
    std::vector<std::vector<std::vector<float>>> neuralNetwork;
    std::function<float(float)> activationFunction;

public:
    NN(int numberOfInputs, int numberOfOutputs, int hiddenLayerDepth, int hiddenLayerWidth, std::string activationFunctionName);
    NN(NN* src);
    float* findElement(unsigned int index);
    unsigned int getNumWeights();
    void cross(NN* other, std::string typeOfCross);
    void mutate(float chanceOfMutation);
    std::vector<float> calculateOutput(std::vector<float> inputs);
};