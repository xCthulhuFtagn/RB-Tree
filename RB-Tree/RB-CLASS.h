#pragma once
#include <iostream>

template <typename Type>
class RBTree;

template <typename Type>
struct RBNode {
	bool is_red;
	unsigned bh; //black height
	Type data;
	RBNode* left, * right, * parent;
public:
	RBNode(Type new_data) {
		data = new_data;
		bh = 0;
		is_red = true;
		left = NULL;
		right = NULL;
		parent = NULL;
	}
	template <typename T>
	friend RBNode<T>* add(RBNode<T>*, T);
	template <typename T>
	friend RBNode<T>* right_rotate(RBNode<T>*);
};

template <typename Type>
class RBTree {
	RBNode <Type> root;
public:
	RBTree() {
		root = nullptr;
	}
	&RBtree add(Type new_elem) {
		root = add(root, new_elem);
		root->is_red = false;
		return *this;
	}
};