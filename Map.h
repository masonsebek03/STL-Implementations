#pragma once

#ifndef MAP_H
#define MAP_H



typedef enum {
	ZERO, // black
	ONE,  // red
} Color;

template<typename K, typename V>
struct RedBlackNode {
	RedBlackNode* parent;
	RedBlackNode* left;
	RedBlackNode* right;
	K key;
	V value;
	Color color;

	RedBlackNode() : parent(nullptr), left(nullptr), right(nullptr), key(nullptr), value(nullptr), color(nullptr){}
	RedBlackNode(Color _color) : parent(nullptr), left(nullptr), right(nullptr), key(nullptr), value(nullptr), color(_color) {}
	RedBlackNode(K _key, Color _color) : parent(nullptr), left(nullptr), right(nullptr), key(_key), value(nullptr), color(_color) {}
	RedBlackNode(K _key, V _value) : parent(nullptr), left(nullptr), right(nullptr), key(_key), value(_value), color(nullptr){}
	RedBlackNode(K _key, V _value, Color _color) : parent(nullptr), left(nullptr), right(nullptr), key(_key), value(_value), color(_color) {}
	RedBlackNode(K _key, V _value, Color _color, RedBlackNode* _parent) : parent(_parent), left(nullptr), right(nullptr), key(_key), value(_value), color(_color) {}
};




template<typename K, typename V>
class Map {
private:
	RedBlackNode<K, V>* root;


public:
	Map() {
		root = new RedBlackNode<K, V>(Color::ZERO);
	}

	V& operator[](K key);

private:
	void insert(K _key, V _value);
	void left_rotate(RedBlackNode<K, V>* node);
	void right_rotate(RedBlackNode<K, V>* node);
	
	void fix_insert(RedBlackNode<K, V>* node);

};

template<typename K, typename V>
V& Map<K, V>::operator[](K key) {
	RedBlackNode* temp = root;
	while (temp != nullptr) {
		if (key < temp->key) {
			temp = temp->left;
		}
		else if (key > temp->key) {
			temp = temp->right;
		}
		else {
			return temp->value;
		}
	}

	return 0;


}

template<typename K, typename V>
void Map<K, V>::fix_insert(RedBlackNode<K, V>* node)
{
	while (node->parent && node->parent->color == Color::ONE) {
		if (node->parent == node->parent->parent->left) {
			RedBlackNode* y = node->parent->parent->right;
			if (y->color == Color::ONE) {
				node->parent->color = Color::ZERO;
				y->color = Color::ZERO;
				node->parent->parent->color = Color::ONE;
				node = node->parent->parent;
			}
			else {
				if (node == node->parent->right) {
					node = node->parent;
					left_rotate(node->parent->parent);
				}
				node->parent->color = Color::ZERO;
				node->parent->parent->color = Color::ONE;
				right_rotate(node->parent->parent);
			}
		}
		else {
			RedBlackNode* y = node->parent->parent->left;
			if (y.color == Color::ONE) {
				node->parent->color = Color::ZERO;
				y->color = Color::ZERO;
				node->parent->parent->color = Color::ONE;
				node = node->parent->parent;
			}
			else {
				if (node == node->parent->left) {
					node = node->parent;
					right_rotate(node);
				}

				node->parent->color == Color::ZERO;
				node->parent->parent->color = Color::ONE;
				left_rotate(node->parent->parent);
			}
		}

		if (node == root) {
			break;
		}

		root->color = Color::ZERO;
	}
}

template<typename K, typename V>
void Map<K, V>::insert(K _key, V _value)
{
	RedBlackNode* ins = new RedBlackNode(_key, _value, Color::ONE);

	RedBlackNode* x = root;
	RedBlackNode* y;

	while (root != nullptr) {
		y = x;
		if (ins->key < x->key) {
			x = x->left;
		}
		else {
			x = x->right;
		}
	}

	ins->parent = y;
	if (y == nullptr) {
		root = ins;
	}
	else if (ins->key < y->key) {
		y->left = ins;
	}
	else {
		y->right = ins;
	}

	// function to fix up insertion.
	fix_insert(ins);
}







#endif
