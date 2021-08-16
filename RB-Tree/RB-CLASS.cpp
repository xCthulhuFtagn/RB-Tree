#include "RB-CLASS.h"
#include <queue>
#include <algorithm>
#include <utility>

using namespace std;

template<typename T>
void ChangeChild(RBNode<T>* prev_child, RBNode<T>* new_one) {
	if (prev) {
		RBNode<T>* parent = prev_child->parent;
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

template<typename T>
void swap_nodes(RBNode<T>* left_swp, RBNode<T>* right_swp) {
	swap(left_swp->left, right_swp->left);
	swap(left_swp->right, right_swp->right);
	swap(left_swp->parent, right_swp->parent);
}

template<typename T>
bool has_1_kid(RBNode<T>* node) {
	return !(node->left) || !(node->right);
}

template <typename func, typename T>
RBNode<T> WidthSearch(RBNode<T>* node, func f) {
	//walking by width, stops if found element with less than two children
	queue<RBNode<T>*> pop;
	RBNode<T>* keeper;
	pop.push(node);
	do {
		pop.push(keeper->left);
		pop.push(keeper->right);
		keeper = pop.pop();
	} while (f(keeper));
	return keeper;
}

template <typename T>
RBNode<T>* add(RBNode<T>*& node, T new_elem) {
	if (node == nullptr) { //Äîáàâëåíèå êðàñíîé âåðøèíû â äåðåâî
		node = new RBNode(new_elem);
		bh += 1;
		return node;
	}
	//Âûáîð ïîääåðåâà, â êîòîðîå íóæíî çàïèõíóòü ýëåìåíò
	if (node->data < new_elem) {
		node->left = add(node->left, new_elem);
		node->left->parent = node;
	} else if (node->data > new_elem) {
		node->right = add(node->right, new_elem);
		node->right->parent = node;
	}
	//Åñëè 2 êðàñíûå âåðøèíû ðÿäîì, òî íóæíî áàëàíñèðîâàòü
	if (node->is_red && node->left->is_red) {
		RBNode<T>* uncle;
		bool an_side;
		//Âûÿñíÿåì êòî äÿäÿ
		if (node->parent->left == node) {
			uncle = node->parent->right;
			an_side = false;
		} else {
			uncle = node->parent->left;
			an_side = true;
		}
		if (uncle != nullptr && uncle->is_red) { //Åñëè äÿäÿ êðàñíûé, ïåðåêðàøèâàåì
			node->parent->is_red = true;
			node->is_red = false;
			uncle->is_red = false;
		} else { //Èíà÷å êðóòèì-âåðòèì è êðàñèì
			node->is_red = false;
			node->parent->is_red = true;
			if (an_side) {
				node = left_rotate(node);
			}
			node = right_rotate(node->parent);
		}
	} else if (node->is_red && node->right->is_red) { //Àíàëîãè÷íî ïðåäûäóùåìó if-ó 
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
			node = left_rotate(node->parent);
		}
	} else {
		return nullptr;
	}
	return node;
}

template <typename T>
RBNode<T>* del(RBNode<T>* input, const T& what) {
	RBNode<T>* node = WidthSearch(input, [&what](RBNode<T>* check) {
		return check->data == what;
	});
	if (node && node->parent) {
		auto left = node->left, right = node->right;
		if (left) {
			if (right) {// l && r
				if (node->is_red) {
					RBNode<T>* keeper = WidthSearch(node, has_1_kid);
					swap_nodes(keeper, node);
					del(node);
				}
				else {
					RBNode<T>* keeper = WidthSearch(node, has_1_kid);
					swap_nodes(keeper, node);
					del(node);
				}
			}
			else {// l && !r
				//red with 1 baby is impossible
				node->data = node->left->data;
				del(node->left);
			}
		}
		else if (right) { // !l && r
			//red with 1 baby is impossible
			node->data = node->right->data;
			del(node->right);
		}
		else { // !l && !r
			if (node->is_red) {
				ChangeChild(node, NULL);
			}
			else {
				RBNode<T>* parent = node->parent;
				bool is_right = parent->right == node;
				//RBNode<T>* brother = (is_right) ? parent->left : parent->right;
				//ffs, too many spins and variants
				if (parent->is_red) {
					if (parent->right == node) { // brother is to the left
						RBNode<T>* brother = parent->left;
						if (!(brother->is_red)) { //КЧ1
							if (!(brother->left->is_red)) {
								parent->is_red = true;
								brother->is_red = false;
							}
							else { //КЧ2
								RBNode<T>* new_parent = right_rotate(parent);
								new_parent->is_red = true;
								new_parent->right->is_red = false;
								new_parent->left->is_red = false;
							}
						}
						else { //check from here, needs to go somewhere else
							RBNode<T>* right_nephew = brother->right;
							if (right_nephew->is_red) {

							}
							else {
								if (right_nephew->left->is_red) {
									//big_right_spin() here 
								}
								else {
									RBNode<T>* new_parent = right_rotate(parent);
									new_parent->is_red = false;
									new_parent->right->left->is_red = true;
								}
							}
						}
					}
					else { // brother is to the right
						RBNode<T>* brother = parent->right;
					}
					parent->right = NULL;
				}
				else {
					if (brother->is_red) {

					}
					else {

					}
				}
			}
		}
		delete node;
		return input;
	}

}

template <typename T>
RBNode<T>* right_rotate(RBNode<T> * node) {
	RBNode<T>* new_node = node->left;
	node->left = new_node->right;
	new_node->parent = node->parent;
	node->parent = new_node;
	new_node = new_node->right;
	new_node->right = node;
	if (new_node->parent != nullptr) {
		if (new_node->parent->left == node)
			new_node->parent->left = new_node;
		else
			new_node->parent->right = new_node;
	}
	if (node->left != nullptr) {
		node->left->parent = node;
	}
	return new_node->left;
}

template <typename T>
RBNode<T>* left_rotate(RBNode<T>* node) {
	if (node != NULL) {
		RBNode<T>* new_node = node->right;
		//настроим родителя
		RBNode<T>* parent = node->parent;
		ChangeChild(node, new_node);
		//передача детей
		RBNode<T>* left_baby = node->left;
		new_node->right = left_baby;
		node->left = new_node;
		if (left_baby) left_baby->parent = new_node;
		return new_node;
	}
	return node;
}