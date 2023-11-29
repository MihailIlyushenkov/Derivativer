#ifndef _TREE
#define _TREE


enum operation {
    add = 0,
    sub = 1,
    mul = 2,
    _div = 3
};

enum nodetype {
    oper = 0,
    value = 1,
    variable = 2
};

struct Node // структура для представления узлов дерева
{
    nodetype type = value;
	int key = 0;
	Node* left = nullptr;
	Node* right = nullptr;

    int verified = 0;
};

Node* create_node(int key);
Node* tree_copy(Node* tree);
Node* node_copy(Node* node);

int insert(Node** p, int k);
int tree_free(Node* tree);
int tree_print(Node* tree);
int remake(Node* tree, int key, nodetype type);
#endif //_TREE
