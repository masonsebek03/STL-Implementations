
#ifndef MAP_H
#define MAP_H


typedef enum {
	RED,
	BLACK
} color_t;

template<typename V>
struct DefaultValue {
	static V& GetDefaultValue() { return V(); }
};


template<typename K, typename V>
struct RBNode {
	RBNode* parent;
	RBNode* right;
	RBNode* left;
	color_t color;
	K _key;
	V _value;
	bool nil;

	RBNode() : parent(nullptr), right(nullptr), left(nullptr), color(BLACK), _key(nullptr), _value(nullptr) {}
	RBNode(color_t _color) : parent(nullptr), right(nullptr), left(nullptr), color(_color), _key(nullptr), _value(nullptr) {}
	RBNode(K k, V v, color_t _color) : parent(nullptr), right(nullptr), left(nullptr), color(_color), _key(k), _value(v) {}

};

template<typename K, typename V>
class Map {
private:
	RBNode<K,V>* root;
	RBNode<K,V>* null_node;

public:
	Map() {
		null_node = new RBNode<K,V>(BLACK);
		null_node->nil = true;
		root = null_node;
	}

	V& operator[](const K& key, const V& value);
	V& operator[](const K& key);

private:
	void insert(K key, V value);
	void fix_insert(RBNode<K, V>* z);
	void right_rotate(RBNode<K, V>* z);
	void left_rotate(RBNode<K, V>* z);
	V& search(const K& key);
};

template<typename K, typename V>
V& Map<K, V>::search(const K& key)
{
	RBNode* begin = root;
	while (begin != null_node) {
		if (key < begin->_key) {
			begin = begin->left;
		}
		else if (key > begin->_key) {
			begin = begin->right;
		}
		else {
			return begin->_value;
		}
	}
	return nullptr;
}

template<typename K, typename V>
V& Map<K, V>::operator[](const K& key, const V& value)
{

}

// search RB tree to find key, otherwise insert with default constructor
template<typename K, typename V>
V& Map<K, V>::operator[](const K& key)
{
	V* result = search(key);
	if (result == nullptr) {
		insert(key, DefaultValue<V>::GetDefaultValue());
		return DefaultValue<V>::GetDefaultValue();
	}

	return result;
}

template<typename K, typename V>
void Map<K, V>::left_rotate(RBNode<K, V>* z)
{
	RBNode* y = z->right;
	z->right = y->left;

	if (y->left != null_node) {
		y->left->parent = z;
	}

	y->parent = z->parent;
	if (z->parent == nullptr) {
		root = y;
	}
	else if (z == z->parent->right) {
		z->parent->left = y;
	}
	else {
		z->parent->right = y;
	}

	y->left = z;
	z->parent = y;
}

template<typename K, typename V>
void Map<K, V>::right_rotate(RBNode<K, V>* z)
{
	RBNode* y = z->left;
	z->left = y->right;

	if (y->right != null_node) {
		y->right->parent = z;
	}

	y->parent = z->parent;
	if (z->parent == nullptr) {
		root = y;
	}
	else if (z == z->parent->right) {
		z->parent->right = y;
	}
	else {
		z->parent->left = y;
	}

	y->right = z;
	z->parent = y;
}

template<typename K, typename V>
void Map<K, V>::fix_insert(RBNode<K, V>* z)
{
	while (z->parent && z->parent->color == RED) {
		// if z's parent if left child of grandparent
		if (z->parent == z->parent->parent->left) {
			RBNode* y = z->parent->parent->right;
			if (y->color == RED) {
				z->parent->color = BLACK;
				y->color = BLACK;
				z->parent->parent->color = RED;
				z = z->parent->parent;
			}
			else {
				if (z == z->parent->right) {
					z = z->parent;
					left_rotate(z);
				}
				z->parent->color = BLACK;
				z->parent->parent->color = RED;
				right_rotate(z->parent->parent);
			}
		}
		else {
			RBNode* y = z->parent->parent->left;
			if (y->color == RED) {
				z->parent->color = BLACK;
				y->color = BLACK;
				z->parent->parent = RED;
				z = z->parent->parent;
			}
			else {
				if (z == z->parent->left) {
					z = z->parent;
					right_rotate(z);
				}
				z->parent->color = BLACK;
				z->parent->parent->color = RED;
				left_rotate(z->parent->parent);
			}
		}

		if (z == root) 
			break;

		root->color = BLACK;
	}
}

// Insert Z and color it red || Recolor and fix any violations
template<typename K, typename V>
void Map<K, V>::insert(K key, V value)
{
	RBNode* z = new RBNode(key, value, RED);
	RBNode* y = nullptr;
	RBNode* x = root;

	while (x != nullptr) {
		y = x;
		if (z->_key < x->_key) {
			x = x->left;
		}
		else {
			x = x->right;
		}
	}

	z->parent = y;

	if (y == nullptr) {
		root = z;
	}
	else if (z->_key < y->_key) {
		y->left = z;
	}
	else {
		y->right = z;
	}

	// some fix up.
	fix_insert(z);
}





#endif