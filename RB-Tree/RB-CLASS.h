#include <queue>
#include <algorithm>
#include <utility>

#pragma once

template <class Type>
struct RBNode;

template <class Type>
class RBTree;

template <class Type>
struct RBNode {
	bool is_red;
	unsigned bh; //black height
	Type data;
	RBNode* left, * right, * parent;
	RBNode(Type new_data) {
		data = new_data;
		bh = 0;
		is_red = true;
		left = NULL;
		right = NULL;
		parent = NULL;
	}
};

template <class Type>
class RBTree {
	RBNode <Type>* root;
public:
	RBTree() {
		root = nullptr;
	}
	RBTree& insert(const Type&);
	RBTree& remove(const Type&);
};