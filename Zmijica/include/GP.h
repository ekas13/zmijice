#include <vector>
#include "GPNode.h"
class GP
{
private:
    struct {
        int maxDepth;
        int minDepth;
        int numOfFunctions;
        GPNode* root;
        int numOfInputs;
    } GPData;
    GPNode* generateTree(int minDepth, int maxDepth); //generateGenotype()
    void generateTreeRecursive(GPNode* node,int minDepth, int maxDepth);
    GPNode* getNodeAt(int nodeNum) const;
    GPNode* getNodeAtRecursive(GPNode* node, int nodeNum, int* currNodeNum) const;
    int countNodes(GPNode* root);
    float evaluateTree(GPNode* root);
    void distributeInputs(GPNode* node, std::vector<float> inputs, int* index);
    GPNode* copyTreeRecursive(GPNode* node);

public:
    GP(int maxDepth, int numOfFunction, int numOfInputs);
    GP(const GP* src);
    ~GP();
    void cross(const GP* other, std::string typeOfCross);
    void mutate(float chanceOfMutation);
    void print(GPNode* node, int depth);
    std::vector<float> calculateOutput(std::vector<float> inputs);
    int getMaxDepth();
    int getNumOfFunctions();
    GPNode* getRoot();
    void setRoot(GPNode* node);
};