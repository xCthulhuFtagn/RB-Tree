#pragma once
#include <iostream>
template <typename Type>
class RBNode {
public:
	RBNode() {
		is_red = 0;
		left = NULL;
		right = NULL;
		parent = NULL;
	}
private:
	bool is_red;
	Type data;
	RBNode *left, *right, *parent;
};