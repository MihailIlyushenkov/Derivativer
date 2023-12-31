#include "Tree.h"
#include <stdlib.h>

int tree_free(Node* tree)
{
    if (!tree) return 0;

	tree_free(tree->left);
	tree_free(tree->right);

    free(tree);
	return 0;
}

Node* create_node(int key, nodetype typ, Node* nodeleft, Node* noderight)
{
    Node* newNode = (Node*) malloc(sizeof(Node));
    newNode->type = typ;
    newNode->verified = 0;
	newNode->key = key;
    newNode->left = nodeleft;
	newNode->right = noderight;
    return newNode;
}

int remake(Node* tree, int key, nodetype type) {
    tree->key = 0;
    tree->type = variable;
    tree_free(tree->left);
    tree_free(tree->right);
    tree->left = nullptr;
    tree->right = nullptr;

    return 0;
}

Node* tree_copy(Node* tree) {
    if (tree == nullptr) {
        return nullptr;
    }
    else {
        Node* NewNode = create_node(tree->key, tree->type, nullptr, nullptr);
        NewNode->left = tree_copy(tree->left);
        NewNode->right = tree_copy(tree->right);
        return NewNode;
    }
}

Node* node_copy(Node* node) {
    if (node == nullptr) return nullptr;
    else {
        Node* NewNode = create_node(node->key, node->type, nullptr, nullptr);
        return NewNode;
    }
}

int insert(Node** p, int k) // вставка ключа k в узел p (возможно нулевой)
{
	if( !p ) {
		*p = create_node(k, variable, nullptr, nullptr);
		return 0;
    }
    else {
        (*p)->key = k;
    }
	return 0;
}




