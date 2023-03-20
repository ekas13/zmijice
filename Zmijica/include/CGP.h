#include <vector>

class CGP
{
private:
    struct {
        int numOfInputs;
        int numOfOutputs;
        int numOfRows;
        int numOfCols;
        int numOfFunctions;
        int numOfFunctionArgs;
        std::vector<int> genotype;
    } CGPData;
    float applyFunction(int function, std::vector<float> inputs, std::vector<int> arguments);
    std::vector<int> generateGenotype();

public:
    CGP(int numOfInputs, int numOfOutputs, int numOfRows, int numOfCols, int numOfFunctions, int numOfFunctionArgs);
    CGP(CGP* src);
    void cross(CGP* other, std::string typeOfCross);
    void mutate(float chanceOfMutation);
    std::vector<float> calculateOutputs(std::vector<float> inputs);
    int getNumOfRows();
    int getNumOfCols();
    int getNumOfFunctions();
    int getNumOfFunctionArgs();
    std::vector<int> getGenotype();
    void setGenotype(std::vector<int> genotype);
};