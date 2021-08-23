#pragma once

#include <algorithm>
#include <utility>

template <class Type>
struct RBNode;

template <class Type>
class RBTree;

template <class Type>
struct RBNode {
	bool is_red;
	//unsigned bh; //black height (unnesASSary parameter)
	Type data;
	RBNode* left, * right, * parent;
	RBNode(){}
	RBNode(Type new_data) {
		data = new_data;
		//bh = 0;
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
	RBNode<Type>* find(const Type&) const;
};