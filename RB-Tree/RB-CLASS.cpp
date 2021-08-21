#include "RB-CLASS.h"
#include <queue>

using namespace std;

template class RBTree<int>;
template struct RBNode<int>;
template<class Type>
void ChangeChild(RBNode<Type>* prev_child, RBNode<Type>* new_one);

template<class Type>
void ChangeChild(RBNode<Type>* prev_child, RBNode<Type>* new_one) {
	if (1) {//prev) {
		RBNode<Type>* parent = prev_child->parent;
		if (new_one) new_one->parent = parent;
		if (parent) {
			if (parent->left == prev_child) {
				parent->left = new_one;
			}
			else {
				parent->right = new_one;
			}
		}
	}
}

void ChangeChild(RBNode<int>* prev_child, RBNode<int>* new_one) {
	if (prev_child) {
		RBNode<int>* parent = prev_child->parent;
		if (new_one) new_one->parent = parent;
		if (parent) {
			if (parent->left == prev_child) {
				parent->left = new_one;
			}
			else {
				parent->right = new_one;
			}
		}
	}
}

template<class Type>
RBNode<Type>* swap_nodes(RBNode<Type>* left_swp, RBNode<Type>* right_swp) {
	swap(left_swp->left, right_swp->left);
	swap(left_swp->right, right_swp->right);
	swap(left_swp->parent, right_swp->parent);
	return left_swp;
}

/*
template<class Type>
bool has_1_kid(RBNode<Type>* node) {
	return !(node->left) || !(node->right);
}

template <typename func, class Type>
RBNode<Type>* WidthSearch(RBNode<Type>* node, func f) {
	//walking by width, stops if found element with less than two children
	queue<RBNode<Type>*> pop;
	RBNode<Type>* keeper;
	pop.push(node);
	do {
		pop.push(keeper->left);
		pop.push(keeper->right);
		keeper = pop.pop();
	} while (!f(keeper));
	return keeper;
}
*/

template <typename func, typename Type>
RBNode<Type>* DepthSearch(RBNode<Type>* node, func f) {
	RBNode<Type>* keeper = node, *check;
	do{
		if (f(keeper)) return keeper;
		while (keeper->left) {
			keeper = keeper->left;
			if (f(keeper)) return keeper;
		}
		if (keeper->right) keeper = keeper->right;
		else if (keeper->parent->parent) {
			check = keeper->parent;
			keeper = keeper->parent->parent->right;
			if (check == keeper) break;
		}
		else break;
	} while (true);
	return NULL;
}


template <typename T>
RBNode<T>* next_key_value(RBNode<T>* node) {
	RBNode<T>* search;
	for (search = node->right; search->left; search = search->left);
	return search;
}

template <class Type>
RBNode<Type>* add(RBNode<Type>* node, const Type& new_elem) {
	if (node == nullptr) { //Добавление вершины
		node = new RBNode<Type>(new_elem);
		return node;
	}
	if (node->data > new_elem) {
		RBNode<Type>* n = add(node->left, new_elem);
		if (n->left == nullptr) {
			n->parent = node;
			node->left = n;
		} else if (n == node->parent) {
			node = n;
		}
	} else if (node->data < new_elem) {
		RBNode<Type>* n = add(node->right, new_elem);
		if (n->right == nullptr) {
			n->parent = node;
			node->right = n;
		} else if (n == node->parent) {
			node = n;
		}
	}
	if (node->left != nullptr && node->is_red && node->left->is_red) {
		RBNode<Type>* uncle;
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
				node = right_rotate(node);
				node = left_rotate(node->parent);
			} else {
				node = right_rotate(node->parent);
			}
		}
	}
	else if (node->right != nullptr && node->is_red && node->right->is_red) {
		RBNode<Type>* uncle;
		bool an_side;
		if (node->parent->left == node) {
			uncle = node->parent->right;
			an_side = true;
		} else {
			uncle = node->parent->left;
			an_side = false;
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
				node = right_rotate(node->parent);
			} else {
				node = left_rotate(node->parent);
			}
		}
	}
	return node;
}

template<typename T>
RBNode<T>* fix_del(RBNode<T>* node) {
	RBNode<T>* parent, *brother;
	while (node->parent) {
		parent = node->parent;
		brother = parent->right;
		if (parent->right == node) { // brother is to the left
			brother = parent->left;
			if (brother->is_red) {
				right_rotate(parent);
				parent->is_red = true;
				parent->parent->is_red = false;
			}
			else { // brother is black
				if (brother->left->is_red) {
					brother->is_red = brother->parent->is_red;
					brother->parent->is_red = false;
					brother->left->is_red = false;
					break;
				}
				else {
					if (brother->right->is_red) {
						brother->is_red = true;
						brother->right->is_red = false;
						left_rotate(brother);
					}
					else {
						brother->is_red = true;
					}
				}
			}
		}
		else {// brother is to the right
			brother = parent->right;
			if (brother->is_red) {
				left_rotate(parent);
				parent->is_red = true;
				parent->parent->is_red = false;
			}
			else { // brother is black
				if (brother->right->is_red) {
					brother->is_red = brother->parent->is_red;
					brother->parent->is_red = false;
					brother->right->is_red = false;
					break;
				}
				else {
					if (brother->left->is_red) {
						brother->is_red = true;
						brother->left->is_red = false;
						right_rotate(brother);
					}
					else {
						brother->is_red = true;
					}
				}
			}
		}
		node = node->parent;
	}
	return node;
}

template <class T>
RBNode<T>* del(RBNode<T>* root, const T& what) {
	RBNode<T>* node = DepthSearch(root, [&what](RBNode<T>* check) {
		return check->data == what;
	});
	if (!node) return root;
	if (node == root) return NULL;
	RBNode<T>* left_child = node->left, *right_child = node->right, *parent = node->parent;
	if (left_child && right_child) {
		RBNode<T>* alternative = next_key_value(node);
		node->data = alternative->data;
		return del(root, alternative->data);
	}
	else if ((uintptr_t)left_child ^ (uintptr_t)right_child) {
		bool is_left = parent->left->data == node->data;
		//here we put only existing child of node instead of it into left/right of parent
		(left_child)? ( (is_left) ? parent->left = left_child : parent->right = left_child ) : ( (is_left) ? parent->left = right_child : parent->right = right_child );
		//delete node;
	}
	else {
		bool is_left = parent->left->data == node->data;
		(is_left) ? parent->left = NULL : parent->right = NULL;
		//delete node;
	}
	//fixing the tree after deleting
	RBNode<T>* victim = node;
	if (!(node->is_red)) node = fix_del(node);
	for(; node->parent; node = node->parent);
	delete victim;
	return node;
}

template <class Type>
RBNode<Type>* right_rotate(RBNode<Type>* node) {
	RBNode<Type>* new_node = node->left;
	node->left = new_node->right;
	new_node->parent = node->parent;
	node->parent = new_node;
	if (node->left != nullptr) {
		node->left->parent = node;
	}
	return new_node;
}

template <class Type>
RBNode<Type>* left_rotate(RBNode<Type>* node) {
	RBNode<Type>* new_node = node->right;
	node->right = new_node->left;
	new_node->parent = node->parent;
	node->parent = new_node;
	new_node->left = node;
	if (node->right != nullptr) {
		node->right->parent = node;
	}
	return new_node;
}

template <class Type>
RBTree<Type>& RBTree<Type>::insert(const Type& new_elem) {
	root = add(root, new_elem);
	root->is_red = false;
	return *this;
}

template <class Type>
RBTree<Type>& RBTree<Type>::remove(const Type& elem) {
	root = del(root, elem);
	return *this;
}