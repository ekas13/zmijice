#include <stdexcept>
#include "GP.h"
#include <string>
#include "Random.h"
#include <iostream>

#define MAX_FUNCTIONS 4
#define CHANCE_TO_BE_INTERNAL_NODE 0.7

GP::GP(const GP* src) {
    this->GPData = src->GPData;
    this->GPData.root = copyTreeRecursive(src->GPData.root);
}

GP::~GP()
{
    if (GPData.root)
        delete GPData.root;
}

GP::GP(int maxDepth, int numOfFunctions, int numOfInputs) {
    if (maxDepth < 1)
        throw std::invalid_argument("one of the arguments was lower than 1");
    if (numOfFunctions > MAX_FUNCTIONS)
        throw std::invalid_argument("invalid number of functions");

    this->GPData.maxDepth = maxDepth;
    this->GPData.minDepth = 1;
    this->GPData.numOfFunctions = numOfFunctions;
    this->GPData.root = GP::generateTree(this->GPData.minDepth,this->GPData.maxDepth);
    this->GPData.numOfInputs = numOfInputs;

}

void GP::print(GPNode* node, int depth) {
    for (int i = 0; i < depth; i++) {
        std::cout << " ";
    }
    std::cout << node->getKey_value();
    std::cout << '\n';
    if (node->getLeft() != nullptr) GP::print(node->getLeft(), depth + 1);
    if (node->getRight() != nullptr) GP::print(node->getRight(), depth + 1);
}

int GP::countNodes(GPNode* root) {
    if (root == nullptr)
        return 0;
    else {
        int count = 1;
        count += countNodes(root->getLeft());
        count += countNodes(root->getRight());
        return count;
    }
}

GPNode* GP::generateTree(int minDepth, int maxDepth) {
    GPNode* root = new GPNode(); //prazno stablo (samo root = nullptr) koje sada nasumicno punimo  

    generateTreeRecursive(root,minDepth,maxDepth);

    return root;
}

void GP::generateTreeRecursive(GPNode* node, int minDepth, int maxDepth) {
    if (minDepth > 0 || maxDepth > 0 && Random::generateFloat() < CHANCE_TO_BE_INTERNAL_NODE) {
        node->setLeft(new GPNode());
        node->setRight(new GPNode());
    }
    if (node->getLeft() != nullptr) {
        //to je cvor - mora biti funkcija [0,3]
        node->setKey_value(Random::generateInt(0, 3));
        generateTreeRecursive(node->getLeft(), minDepth - 1, maxDepth - 1);
        generateTreeRecursive(node->getRight(), minDepth - 1, maxDepth - 1);
    }
    else {
        //to je list - moze biti random konstanta ili input
        node->setKey_value(3); // nesto od toga dvoje //GP::getRandomInt(8, 16)
    }
}

void GP::mutate(float chanceOfMutation) {
    if (fabs(Random::generateFloat()) < chanceOfMutation) {
        int mutatingNodeNum = Random::generateInt(1, countNodes(this->GPData.root));
        GPNode* mutatingNode = this->getNodeAt(mutatingNodeNum);

        //treba obrisati memoriju mutation
        GPNode* mutation = GP::generateTree(0, this->GPData.maxDepth - 1);         // generira se novo random podstablo
        mutatingNode->setKey_value(mutation->getKey_value());

        //obrisan prijasnji dio stabla prije zamijene
        delete mutatingNode->getLeft();
        delete mutatingNode->getRight();

        if (mutation->getLeft())
            mutatingNode->setLeft(new GPNode(mutation->getLeft()));
        else
            mutatingNode->setLeft(nullptr);

        if (mutation->getRight())
            mutatingNode->setRight(new GPNode(mutation->getRight()));
        else
            mutatingNode->setRight(nullptr);

        //obrisan mutation
        delete mutation;
    }
}

GPNode* GP::getNodeAt(int nodeNum) const {
    int currentNodeNum = 0;
    int* currNodeNum = &currentNodeNum;
    return getNodeAtRecursive(this->GPData.root, nodeNum, currNodeNum);
}

GPNode* GP::getNodeAtRecursive(GPNode* node, int nodeNum, int* currNodeNum) const {
    ++(*currNodeNum);
    if (*currNodeNum == nodeNum)
        return node;
    else {
        if (node->getLeft() != nullptr) {
            GPNode* temp = getNodeAtRecursive(node->getLeft(), nodeNum, currNodeNum);
            if (temp != nullptr) return temp;
        }

        if (node->getRight() != nullptr) {
            GPNode* temp = getNodeAtRecursive(node->getRight(), nodeNum, currNodeNum);
            if (temp != nullptr) return temp;
        }
    }
    return nullptr;
}

GPNode* GP::copyTreeRecursive(GPNode* node) {

    if (node == nullptr) {
        return nullptr;
    }

    GPNode* root_copy = new GPNode();
    root_copy->setKey_value(node->getKey_value());

    root_copy->setLeft(copyTreeRecursive(node->getLeft()));
    root_copy->setRight(copyTreeRecursive(node->getRight()));

    return root_copy;
}

void GP::cross(const GP* other, std::string typeOfCross) {

    if (typeOfCross == "disable")
        return;

    //ekcplicitno su stvoreni 2 GP-a, a implicitno njihova stabla
    GP* copyParent1 = new GP(this);
    //donja linija bi bila memory leak (vec se izvrsava u konstruktoru pa nije potrebna ovdje)
    //copyParent1->GPData.root = copyTreeRecursive(this->GPData.root);

    GP* copyParent2 = new GP(other);
    //i ovo isto
    //copyParent2->GPData.root = copyTreeRecursive(other->GPData.root);

    //buduci da ce neka od ovih kopija zamijeniti original brisemo original
    delete this->GPData.root;

    int breakingPointParent1 = Random::generateInt(2, countNodes(copyParent1->GPData.root));       // mjesto crossingovera stabala
    int breakingPointParent2 = Random::generateInt(2, countNodes(copyParent2->GPData.root));
    int initialModel = Random::generateInt(0, 1);

    GPNode* chosenNodeParent1 = copyParent1->getNodeAt(breakingPointParent1);
    GPNode* chosenNodeParent2 = copyParent2->getNodeAt(breakingPointParent2);
    GPNode* temp = new GPNode();

    temp->setKey_value(chosenNodeParent1->getKey_value());
    temp->setLeft(chosenNodeParent1->getLeft());
    temp->setRight(chosenNodeParent1->getRight());

    chosenNodeParent1->setKey_value(chosenNodeParent2->getKey_value());
    chosenNodeParent1->setLeft(chosenNodeParent2->getLeft());
    chosenNodeParent1->setRight(chosenNodeParent2->getRight());
    chosenNodeParent2->setKey_value(temp->getKey_value());
    chosenNodeParent2->setLeft(temp->getLeft());
    chosenNodeParent2->setRight(temp->getRight());

    //sada treba obrisati temp ali ne i njegova podstabla
    //buduci da njihove pokazivace koriste druga stabla
    temp->setLeft(nullptr);
    temp->setRight(nullptr);
    delete temp;


    //lijeva strana od ovog modela, desna od other
    if (initialModel == 0) {
        //podstablo od this smo vec obrisali
        this->GPData.root = copyParent1->GPData.root;
        //sada treba obrisati copyParent1 ali ne i njegovo podstablo
        copyParent1->GPData.root = nullptr;
        delete copyParent1;
        //onaj drugi treba obrisati potpuno
        delete copyParent2;
    }
    //lijeva strana od other modela, desna od ovog
    else {
        this->GPData.root = copyParent2->GPData.root;
        //onaj drugi treba obrisati

        copyParent2->GPData.root = nullptr;
        delete copyParent2;

        delete copyParent1;
    }
}

void GP::distributeInputs(GPNode* node, std::vector<float> inputs, int* index) {
    if (*index == inputs.size())
        *index = 0;
    if (node->getLeft() == nullptr) {
        node->setKey_value(inputs.at(*index));
        (*index)++;
    }
    else {
        distributeInputs(node->getLeft(), inputs, index);
        distributeInputs(node->getRight(), inputs, index);
    }
}

std::vector<float> GP::calculateOutput(std::vector<float> inputs) {
    int index = 0;
    int* p_index = &index;
    GP::distributeInputs(this->GPData.root, inputs,p_index);
    std::vector<float> rtrn;
    rtrn.push_back(GP::evaluateTree(this->GPData.root));

    return rtrn;
}

float GP::evaluateTree(GPNode* root) {
    //if (root == nullptr)
        //return 0.0;

    if (root->getLeft() == nullptr && root->getRight() == nullptr)
        return (float)root->getKey_value();

    // Evaluate left subtree
    float l_val = evaluateTree(root->getLeft());

    // Evaluate right subtree
    float r_val = evaluateTree(root->getRight());

    // Check which operator to apply
    if (root->getKey_value() == 0) // npr. "+"
        return (float)(l_val + r_val);

    if (root->getKey_value() == 1)
        return (float)(l_val - r_val);

    if (root->getKey_value() == 2)
        return (float)l_val * (float)r_val;

    // baci jednu provjeru za nulu !
    if (root->getKey_value() == 3)
        if ((float)r_val == 0) r_val = (float)1;
        return (float)l_val / (float)r_val;
    
    if (root->getKey_value() == 4)
        return (float)(std::min((float)l_val, (float)r_val));

    if (root->getKey_value() == 5)
        return (float)(std::max((float)l_val, (float)r_val));
}

int GP::getMaxDepth() {

    return this->GPData.maxDepth;
}

int GP::getNumOfFunctions() {

    return this->GPData.numOfFunctions;
}

GPNode* GP::getRoot() {

    return this->GPData.root;
}

void GP::setRoot(GPNode* node)
{
    delete this->GPData.root;
    this->GPData.root = node;
}