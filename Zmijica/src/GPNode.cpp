#include "GPNode.h"
#include <cstddef>

GPNode::GPNode() {
	this->key_value = 0;
	this->left = nullptr;
	this->right = nullptr;
}

GPNode::GPNode(const GPNode* src)
{
	this->key_value = src->key_value;

	if (src->left)
		this->left = new GPNode(src->left);
	else
		this->left = nullptr;

	if (src->right)
		this->right = new GPNode(src->right);
	else
		this->right = nullptr;
}

GPNode::~GPNode() {
	if (left != nullptr)
		delete left;
	if (right != nullptr)
		delete right;
}

void GPNode::setKey_value(float key_value) {
	this->key_value = key_value;
}

void GPNode::setLeft(GPNode* left) {
	this->left = left;
}

void GPNode::setRight(GPNode* right) {
	this->right = right;
}

GPNode* GPNode::getLeft() {
	return this->left;
}
GPNode* GPNode::getRight() {
	return this->right;
}
float GPNode::getKey_value() {
	return this->key_value;
}

