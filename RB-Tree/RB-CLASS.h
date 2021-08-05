#pragma once
template <typename Type>
class RBNode {
	bool is_red;
	Type data;
	RBNode *left, *right, *parent;
};