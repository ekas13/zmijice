#pragma once
class GPNode
{
private:
    int key_value;
    GPNode* left;
    GPNode* right;

public:
    GPNode();
    GPNode(const GPNode* src);
    ~GPNode();

    void setLeft(GPNode* left);
    void setRight(GPNode* right);
    void setKey_value(int key_value);
    GPNode* getLeft();
    GPNode* getRight();
    int getKey_value();

};