#include "RB-CLASS.h"

template <typename T>
RBNode<T>* add(RBNode<T>* node, T new_elem) {
	if (node == nullptr) {
		node = new RBNode(new_elem);
		bh += 1;
		return node;
	}
	if (node->data < new_elem) {
		node->left = add(node->left, new_elem);
	} else if (node->data > new_elem) {
		node->right = add(node->right, new_elem);
	}
	if (node->is_red && node->left->is_red) {
		RBNode<T>* uncle;
		bool an_side;
		if (node->parent->left == node) {
			uncle = node->parent->right;
			an_side = false;
		} else {
			uncle = node->parent->left;
			an_side = true;
		}
		if (uncle != nullptr && uncle->is_red) {
			node->parent->is_red = true;
			node->is_red = false;
			uncle->is_red = false;
		} else {
			node->is_red = false;
			node->parent->is_red = true;
			if (an_side) {
				node = left_rotate(node);
			}
			node = big_right_rotate(node->parent);
		}
	} else if (node->is_red && node->right->is_red) {
		RBNode<T>* uncle;
		bool on_side;
		if (node->parent->left == node) {
			uncle = node->parent->right;
			an_side = true;
		}
		else {
			uncle = node->parent->left;
			an_side = false;
		}
		if (uncle != nullptr && uncle->is_red) {
			node->parent->is_red = true;
			node->is_red = false;
			uncle->is_red = false;
		}
		else {
			node->is_red = false;
			node->parent->is_red = true;
			if (an_side) {
				node = right_rotate(node);
			}
			node = big_left_rotate(node->parent);
		}
	}
}
