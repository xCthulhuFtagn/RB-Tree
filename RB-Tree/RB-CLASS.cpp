#include "RB-CLASS.h"
#include <queue>

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

template<class Type>
RBNode<Type>* swap_nodes(RBNode<Type>* left_swp, RBNode<Type>* right_swp) {
	swap(left_swp->left, right_swp->left);
	swap(left_swp->right, right_swp->right);
	swap(left_swp->parent, right_swp->parent);
	return left_swp;
}

template<class Type>
bool has_1_kid(RBNode<Type>* node) {
	return !(node->left) || !(node->right);
}

/*
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
	RBNode<Type>* keeper = node, check;
	do{
		if (f(keeper)) return keeper;
		while (keeper->left) {
			keeper = keeper.left;
			if (f(keeper)) return keeper;
		}
		if (keeper->right) keeper = keeper->right;
		else if (keeper->parent->parent) {
			check = keeper->parent;
			keeper = keeper->parent->parent->right;
		}
		else break;
	} while (check != keeper);
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

template <typename T>
RBNode<T>* fix_del(RBNode<T>* node) {
	while (node->parent) {
		auto left = node->left, right = node->right;
		if (left) {
			if (right) {// l && r
				if (node->is_red) {
					RBNode<T>* keeper = WidthSearch(node, has_1_kid);
					node = swap_nodes(node, keeper);
				}
				else {
					RBNode<T>* keeper = WidthSearch(node, has_1_kid);
					node = swap_nodes(node, keeper);
				}
			}
			else {// l && !r
				//red with 1 baby is impossible
				node->data = node->left->data;
			}
		}
		else if (right) { // !l && r
			//red with 1 baby is impossible
			node->data = node->right->data;
		}
		else { // !l && !r
			if (node->is_red) { // Ч0
				ChangeChild(node, NULL);
			}
			else {
				RBNode<T>* parent = node->parent;
				bool is_right = parent->right == node;
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
					}
					else { // brother is to the right
						RBNode<T>* brother = parent->right;
						if (!(brother->is_red)) { //КЧ1
							if (!(brother->left->is_red)) {
								parent->is_red = true;
								brother->is_red = false;
							}
							else { //КЧ2
								RBNode<T>* new_parent = left_rotate(parent);
								new_parent->is_red = true;
								new_parent->right->is_red = false;
								new_parent->left->is_red = false;
							}
						}
					}
				}
				else { // parent is black
					if (parent->right == node) { // brother is to the left
						RBNode<T>* brother = parent->left;
						if (brother->is_red) {
							RBNode<T>* right_nephew = brother->right;
							if (!(right_nephew->left->is_red) && !(right_nephew->right->is_red)) { //ЧК3
								RBNode<T>* new_parent = right_rotate(parent);
							}
							else { //ЧК4
								// big_right_rotate here
								RBNode<T>* new_brother = left_rotate(brother);
								RBNode<T>* new_parent = right_rotate(parent);
								// end of it
								new_parent->left->right->is_red = false;
							}
						}
						else {
							if (RBNode<T>* right_nephew = brother->right->is_red) { //ЧЧ5
								right_nephew->is_red = false;
								// big_right_rotate here
								RBNode<T>* new_brother = left_rotate(brother);
								RBNode<T>* new_parent = right_rotate(parent);
								// end of it
							}
							else {

							}
						}
					}
					else { // brother is to the right
						RBNode<T>* brother = parent->right;
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
	if (node == root) return NULL;
	RBNode<T>* left_child = node->left, *right_child = node->right, *parent = node->parent;
	if (left_child && right_child) {
		RBNode<T>* alternative = next_key_value(node);
		node->data = alternative->data;
		return del(root, alternative->data);
	}
	else if (left_child xor right_child) {
		bool is_left = parent->left.data == node->data;
		//here we put only existing child of node instead of it into left/right of parent
		(left_child)? ( (is_left) ? parent->left = left_child : parent->right = left_child ) : ( (is_left) ? parent->left = right_child : parent->right = right_child );
		delete node;
	}
	else {
		bool is_left = parent->left.data == node->data;
		(is_left) ? parent->left = NULL : parent->right = NULL;
		delete node;
	}
	//fixing the tree after deleting
	if (!(node->is_red)) root = fix_del(node);
	//if fixing is not needed we return root of the tree
	else for(; node->parent; node = node->parent);
	return node;
}

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

template <class Type>
RBTree<Type>& RBTree<Type>::remove(const Type& elem) {
	root = del(root, elem);
	return *this;
}