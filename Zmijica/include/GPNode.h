#pragma once
class GPNode
{
private:
    float key_value;
    GPNode* left;
    GPNode* right;

public:
    GPNode();
    GPNode(const GPNode* src);
    ~GPNode();

    void setLeft(GPNode* left);
    void setRight(GPNode* right);
    void setKey_value(float key_value);
    GPNode* getLeft();
    GPNode* getRight();
    float getKey_value();

};