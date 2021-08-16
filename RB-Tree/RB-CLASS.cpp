#include "RB-CLASS.h"

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
		new_node->parent = parent;
		node->parent = new_node;
		if (parent) {
			if (parent->left == node) {
				parent->left = new_node;
			}
			else {
				parent->right = new_node;
			}
		}
		//передача детей
		RBNode<T>* left_baby = node->left;
		new_node->right = left_baby;
		node->left = new_node;
		if (left_baby) left_baby->parent = new_node;
		return new_node;
	}
	return node;
}