#include "RB-CLASS.h"

using namespace std;

template class RBTree<int>;
template struct RBNode<int>;
template class RBTree<double>;
template struct RBNode<double>;
template class RBTree<char>;
template struct RBNode<char>;

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
/*
template<class Type>
void swap_nodes(RBNode<Type>* left_swp, RBNode<Type>* right_swp) {
	swap(left_swp->left, right_swp->left);
	swap(left_swp->right, right_swp->right);
	swap(left_swp->parent, right_swp->parent);
}

template<class Type>
bool has_1_kid(RBNode<Type>* node) {
	return !(node->left) || !(node->right);
}

template <class func, class Type>
RBNode<Type>* WidthSearch(RBNode<Type>* node, func f) {
	//walking by width, stops if found element with less than two children
	queue<RBNode<Type>*> pop;
	RBNode<Type>* keeper;
	pop.push(node);
	do {
		pop.push(keeper->left);
		pop.push(keeper->right);
		keeper = pop.pop();
	} while (f(keeper));
	return keeper;
}
*/

template <class Type>
RBNode<Type>* add(RBNode<Type>* node, const Type& new_elem) {
	if (node == nullptr) { //Äîáàâëåíèå êðàñíîé âåðøèíû â äåðåâî
		node = new RBNode<Type>(new_elem);
		return node;
	}
	//Âûáîð ïîääåðåâà, â êîòîðîå íóæíî çàïèõíóòü ýëåìåíò
	if (node->data > new_elem) {
		RBNode<Type>* n = add(node->left, new_elem);
		if (node->bh == 0 || n->parent == node) {
			node->left = n;
			n->parent = node;
		}
		else node = n;
	} else if (node->data < new_elem) {
		RBNode<Type>* n = add(node->right, new_elem);
		if (node->bh == 0 || n->parent == node) {
			node->right = n;
			n->parent = node;
		}
		else node = n;
	}
	//Åñëè 2 êðàñíûå âåðøèíû ðÿäîì, òî íóæíî áàëàíñèðîâàòü
	if (node->left != nullptr && node->is_red && node->left->is_red) {
		RBNode<Type>* uncle;
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
			node->bh += 1;
			uncle->is_red = false;
			uncle->bh += 1;
		} else { //Èíà÷å êðóòèì-âåðòèì è êðàñèì
			node->is_red = false;
			node->bh += 1;
			node->parent->is_red = true;
			node->parent->bh -= 1;
			if (an_side) {
				node = left_rotate(node);
			}
			node = right_rotate(node->parent);
		}
	} else if (node->right != nullptr && node->is_red && node->right->is_red) { //Àíàëîãè÷íî ïðåäûäóùåìó if-ó 
		RBNode<Type>* uncle;
		bool an_side;
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
			node->bh += 1;
			uncle->is_red = false;
			uncle->bh += 1;
		}
		else {
			node->is_red = false;
			node->bh += 1;
			node->parent->is_red = true;
			node->parent->bh -= 1;
			if (an_side) {
				node = right_rotate(node);
			}
			node = left_rotate(node->parent);
		}
	}
	return node;
}
/*
template <class Type>
RBNode<Type>* del(RBNode<Type>* input, const Type& what) {
	RBNode<Type>* node = WidthSearch(input, [&what](RBNode<Type>* check) {
		return check->data == what;
	});
	if (node && node->parent) {
		auto left = node->left, right = node->right;
		if (left) {
			if (right) {// l && r
				if (node->is_red) {
					RBNode<Type>* keeper = WidthSearch(node, has_1_kid);
					swap_nodes(keeper, node);
					del(node);
				}
				else {
					RBNode<Type>* keeper = WidthSearch(node, has_1_kid);
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
				RBNode<Type>* parent = node->parent;
				bool is_right = parent->right == node;
				//RBNode<Type>* brother = (is_right) ? parent->left : parent->right;
				//ffs, too many spins and variants
				if (parent->is_red) {
					if (parent->right == node) { // brother is to the left
						RBNode<Type>* brother = parent->left;
						if (!(brother->is_red)) { //КЧ1
							if (!(brother->left->is_red)) {
								parent->is_red = true;
								brother->is_red = false;
							}
							else { //КЧ2
								RBNode<Type>* new_parent = right_rotate(parent);
								new_parent->is_red = true;
								new_parent->right->is_red = false;
								new_parent->left->is_red = false;
							}
						}
						else { //check from here, needs to go somewhere else
							RBNode<Type>* right_nephew = brother->right;
							if (right_nephew->is_red) {

							}
							else {
								if (right_nephew->left->is_red) {
									//big_right_spin() here 
								}
								else {
									RBNode<Type>* new_parent = right_rotate(parent);
									new_parent->is_red = false;
									new_parent->right->left->is_red = true;
								}
							}
						}
					}
					else { // brother is to the right
						RBNode<Type>* brother = parent->right;
					}
					parent->right = NULL;
				}
				else {
					if (1) {//brother->is_red

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
*/

template <class Type>
RBNode<Type>* right_rotate(RBNode<Type> * node) {
	RBNode<Type>* new_node = node->left;
	node->left = new_node->right;
	new_node->parent = node->parent;
	node->parent = new_node;
	if (node->left != nullptr) {
		node->left->parent = node;
	}
	return new_node;
}

/*
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
*/

/*
template <class Type>
RBNode<Type>* left_rotate(RBNode<Type>* node) {
	if (node != NULL) {
		RBNode<Type>* new_node = node->right;
		//настроим родителя
		RBNode<Type>* parent = node->parent;
		ChangeChild(node, new_node);
		//передача детей
		RBNode<Type>* left_baby = node->left;
		new_node->right = left_baby;
		node->left = new_node;
		if (left_baby) left_baby->parent = new_node;
		return new_node;
	}
	return node;
}
*/

template <class Type>
RBTree<Type>& RBTree<Type>::insert(const Type& new_elem) {
	root = add(root, new_elem);
	root->is_red = false;
	return *this;
}

/*
template <class Type>
RBTree<Type>& RBTree<Type>::remove(const Type& elem) {
	root = del(root, elem);
	return *this;
}
*/