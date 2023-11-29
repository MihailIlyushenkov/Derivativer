#include <stdio.h>
#include <stdlib.h>
#include "Tree.h"
int MAXTREEDEEP = 20;
int wassimplified = 0;

int get_op_priority(operation a) {
    switch(a) {
        case add: return 0;
        case sub: return 0;
        case mul: return 1;
        case _div: return 2;
        default: return -1;
    }
}

int is_operation(int a) {
    switch(a) {
        case '+': return 1;
        case '-': return 1;
        case '*': return 1;
        case '/': return 1;
        default: return 0;
    }
}

int symb_operation_to_int(int a) { //transforms char to enum int value
    switch(a) {
        case '+': return 0;
        case '-': return 1;
        case '*': return 2;
        case '/': return 3;
        default: printf("you idiot, %d is not operation\n", a); return 0;
    }
}

operation int_to_operation(int a) { //transforms int to enum
    switch(a) {
        case 0: return add;
        case 1: return sub;
        case 2: return mul;
        case 3: return _div;
        default: printf("you idiot, %d is not operation here\n", a); return add;
    }
}

int operation_to_char(int a) {
    switch(a) {
        case 0: return '+';
        case 1: return '-';
        case 2: return '*';
        case 3: return '/';
        default: printf("you idiot, %d is not operation\n", a); return 0;
    }
}

int tree_print(Node* tree)
{
	if (tree == nullptr) {
		return 0;
	}

    if ((tree->left != nullptr) && (tree->right != nullptr)) {
        if ((tree->left->type == oper) && (tree->type == oper)) {
            if (get_op_priority(int_to_operation(tree->left->key)) < get_op_priority(int_to_operation(tree->key))) {
                printf("(");
                tree_print(tree->left);
                printf(")");
            }
            else tree_print(tree->left);
        }
        else tree_print(tree->left);
    }

    if (tree->type == oper) {
        // printf("operation:");
        printf(" %c ", operation_to_char(tree->key));
    }
    else if (tree->type == value) {
        // printf("value:");
        printf(" %d ", tree->key);
    }
    else printf("x");

    if ((tree->left != nullptr) && (tree->right != nullptr)) {
        if ((tree->right->type == oper) && (tree->type == oper)) {
            if (get_op_priority(int_to_operation(tree->right->key)) < get_op_priority(int_to_operation(tree->key))) {
                printf("(");
                tree_print(tree->right);
                printf(")");
            }
            else tree_print(tree->right);
        }
        else tree_print(tree->right);

    }


	return 0;
}

int MAXNAMESIZE = 100;

int _tree_read (Node** tree, FILE* file, int reclevel)
{
    printf("\ntree_read iteration, level %d\n", reclevel);
	int a = 0;
    int b = 0;
    int pbread = ftell(file);
	a = fgetc(file);

    printf("readed: %c, pos is %d\n", a, pbread);

	switch (a) {
    case '(':
                *tree = create_node(0);
                b = fgetc(file);
                if ((b < '0') || ('9' < b)) {
                    _tree_read(&((*tree)->left), file, reclevel+1);
                }
                else {
                    fseek(file, -1, SEEK_CUR);
                }

                printf("\nreturned on prev rec level %d\n", reclevel);
                a = fgetc(file);
                printf("readed: %c\n", a);

                if (is_operation(a) == 1) {
                    printf("found operation\n");
                    (*tree)->key = symb_operation_to_int(a);
                    (*tree)->type = oper;
                }
                else if (a == 'x'){
                    printf("found variable (wrong spot)\n");
                    (*tree)->key = 0;
                    (*tree)->type = variable;
                }
                else {
                    printf("fseek\n");
                    fseek(file, -1, SEEK_CUR);
                    if (fscanf(file, "%d", &a) == 1) {
                        printf("found value %d\n", a);
                        (*tree)->key = a;
                        (*tree)->type = value;
                        fgetc(file);
                    }
                    else {
                        printf("cant read value (wrong spot maybe)\n");
                    }
                }

                if ((b < '0') || ('9' < b)) {
                    _tree_read(&((*tree)->left), file, reclevel+1);
                }
                return 0;

	case ')':   return 0;
    // case 0:     return 0;

	default:    *tree = create_node(0);
                printf("readed: %c in default\n", a);
                if (is_operation(a) == 1) {
                    printf("found operation\n");
                    (*tree)->key = symb_operation_to_int(a);
                    (*tree)->type = oper;
                }
                else if (a == 'x'){
                    printf("found variable\n");
                    (*tree)->key = 0;
                    (*tree)->type = variable;
                }
                else {
                    printf("fseek\n");
                    fseek(file, -1, SEEK_CUR);
                    if (fscanf(file, "%d", &a) == 1) {
                        printf("found value %d\n", a);
                        (*tree)->key = a;
                        (*tree)->type = value;
                        printf("after reading got %c\n", fgetc(file));
                    }
                    else {
                        printf("cant read value\n");
                    }
                }
                return 0;
	}
    return 0;
}

int tree_read(Node** tree, const char* filename) {
    FILE* file = fopen(filename, "r");
    if (file == 0) return printf("cant open file"), 0;
    _tree_read(tree, file, 0);
    fclose(file);
    return 0;
}

int _simplify_constants(Node* tree) { //only for nodes of operation type, where left and right are values
    switch(int_to_operation(tree->key)) {
        case add: tree->key = tree->left->key + tree->right->key; break;
        case sub: tree->key = tree->left->key - tree->right->key; break;
        case mul: tree->key = tree->left->key * tree->right->key; break;
        case _div: tree->key = tree->left->key / tree->right->key; break;
    }
    tree->type = value;
    tree_free(tree->left);
    tree_free(tree->right);
    tree->left = nullptr;
    tree->right = nullptr;

    wassimplified = 1;

    return 0;
}

// int remove_neutral_element(Node* tree)

int can_calc(Node* tree) { //only for nodes of operation type
    if ((tree->left == nullptr) || (tree->right == nullptr)) {
        printf("something is very wrong here");
        return 0;
    }
    else if ((tree->left->type == value) && (tree->right->type == value)) {
        return 1;
    }
    else return 0;
}

int is_possible_rem_neutral_el(Node* tree) { //only for nodes of operation type where 1 subtree has type var
    if (( (tree->left->type == variable) || (tree->left->type) == oper) && (tree->right->type == value)) {
        return 1;
    }
    else if (( (tree->right->type == variable) || (tree->right->type) == oper) && (tree->left->type == value)) {
        return 1;
    }
    else return 0;
}

int try_rem_neutral_el (Node* tree) {
    printf("remnel");
    Node* NewLeft = nullptr;
    Node* NewRight = nullptr;

    int changed = 0;
    if (tree->right->type == value) {
        switch(int_to_operation(tree->key)) {
            case add:   if(tree->right->key == 0) {
                            NewLeft = tree_copy(tree->left->left);
                            NewRight = tree_copy(tree->left->right);
                            changed = 1;
                        }
                        break;
            case sub:   if(tree->right->key == 0) {
                            NewLeft = tree_copy(tree->left->left);
                            NewRight = tree_copy(tree->left->right);
                            changed = 1;
                        }
                        break;
            case mul:   if(tree->right->key == 0) {
                            remake(tree, 0, value);
                            wassimplified = 1;
                        }
                        else if (tree->right->key == 1) {
                            NewLeft = tree_copy(tree->left->left);
                            NewRight = tree_copy(tree->left->right);
                            changed = 1;
                        }
                        break;
            default:    break;
        }

        if (changed) {
            tree->key = tree->left->key;
            tree->type = tree->left->type;
            tree->left = NewLeft;
            tree->right = NewRight;
            wassimplified = 1;
            //free
        }
        return 0;
    }
    else if (tree->left->type == value) {
        switch(int_to_operation(tree->key)) {
            case add:   if(tree->left->key == 0) {
                            NewLeft = tree_copy(tree->right->left);
                            NewRight = tree_copy(tree->right->right);
                            changed = 1;
                        }
                        break;
            case sub:   if(tree->left->key == 0) {
                            NewLeft = tree_copy(tree->right->left);
                            NewRight = tree_copy(tree->right->right);
                            changed = 1;
                        }
                        break;
            case mul:   if(tree->left->key == 0) {
                            remake(tree, 0, value);
                            wassimplified = 1;
                        }
                        else if (tree->left->key == 1) {
                            NewLeft = tree_copy(tree->right->left);
                            NewRight = tree_copy(tree->right->right);
                            changed = 1;
                        }
                        break;
        }
        if (changed) {
            tree->key = tree->right->key;
            tree->type = tree->right->type;
            tree->left = NewLeft;
            tree->right = NewRight;
            wassimplified = 1;
            //free
        }
        return 0;
    }

    return 0;
}

int simplify_constants(Node* tree) {
    if ((tree->type == value) || (tree->type == variable)) {
        return 0;
    }
    else if (can_calc(tree)) {
        _simplify_constants(tree);
    }
    else {
        simplify_constants(tree->left);
        simplify_constants(tree->right);

        if (can_calc(tree)) _simplify_constants(tree);
    }
    return 0;
}

int simplify_neutral_elements(Node* tree) {
    printf("smnel");
    if ((tree->type == value) || (tree->type == variable)) {
        return 0;
    }
    else if (is_possible_rem_neutral_el(tree)) {
        try_rem_neutral_el(tree);
    }
    else {
        simplify_neutral_elements(tree->left);
        simplify_neutral_elements(tree->right);

        if (is_possible_rem_neutral_el(tree)) try_rem_neutral_el(tree);
    }
    return 0;
}

int simplify(Node* tree) {
    wassimplified = 0;
    simplify_constants(tree);
    simplify_neutral_elements(tree);
    if (wassimplified != 0) {
        simplify(tree);
    }
    return 0;
}

Node* derivative(Node* tree) {
    printf("derivating ");
    if (tree->type == oper) {
        printf("operation %c\n", operation_to_char(tree->key));
    }
    else if (tree->type == variable) {
        printf("variable %d\n", tree->key);
    }
    else if (tree->type == value) {
        printf("value %d\n", tree->key);
    }
    else printf("smth is broken\n");

    Node* NewNode = nullptr;

    if (tree->type == value) {
        printf("val\n");
        NewNode = create_node(0);
    }
    else if (tree->type == variable) {
        printf("var\n");
        NewNode = create_node(1);
    }
    else if (tree->type == oper) {
        printf("oper\n");
        if ((int_to_operation(tree->key) == add) || (int_to_operation(tree->key) == sub)) {
            printf("add or sub\n");
            NewNode = node_copy(tree);
            NewNode->left = derivative(tree_copy(tree->left));
            NewNode->right = derivative(tree_copy(tree->right));
        }
        else if (int_to_operation(tree->key) == mul) {
            printf("mul\n");
            NewNode = create_node(symb_operation_to_int('+'));
            NewNode->left = create_node(symb_operation_to_int('*'));
            NewNode->right = create_node(symb_operation_to_int('*'));

            NewNode->type = oper;
            NewNode->left->type = oper;
            NewNode->right->type = oper;

            NewNode->left->left = derivative(tree_copy(tree->left));
            NewNode->left->right = tree_copy(tree->right);

            NewNode->right->right = derivative(tree_copy(tree->right));
            NewNode->left->left = tree_copy(tree->left);

        }

        else if (int_to_operation(tree->key) == _div) {
            printf("div\n");
            NewNode = create_node(symb_operation_to_int('/'));
            NewNode->left = create_node(symb_operation_to_int('-'));
            NewNode->right = create_node(symb_operation_to_int('*'));

            NewNode->left->left = create_node(symb_operation_to_int('*'));
            NewNode->left->right = create_node(symb_operation_to_int('*'));

            NewNode->type = oper;
            NewNode->left->type = oper;
            NewNode->right->type = oper;
            NewNode->left->left->type = oper;
            NewNode->left->right->type = oper;

            NewNode->left->left->left = derivative(tree_copy(tree->left));
            NewNode->left->left->right = tree_copy(tree->right);
            NewNode->left->right->left = tree_copy(tree->left);
            NewNode->left->right->right = derivative(tree_copy(tree->right));

            NewNode->right->right = tree_copy(tree->right);
            NewNode->right->left = tree_copy(tree->right);

        }
        else printf("operation type is broken\n");
    }
    else printf("smth is broken\n");
    // printf("free");
    // tree_free(tree);
    printf("return");
    return NewNode;
}

int main()
{

    Node* tree = create_node(symb_operation_to_int('/'));
    tree->type = oper;
    tree->left = create_node(symb_operation_to_int('+'));
    tree->left->type = oper;
    tree->right = create_node(symb_operation_to_int('-'));
    tree->right->type = oper;

    tree->left->left = create_node(0);
    tree->left->left->type = variable;
    tree->left->right = create_node(3);

    tree->right->left = create_node(0);
    tree->right->left->type = variable;
    tree->right->right = create_node(2);

    // Node* tree;
    // tree_read(&tree, "Tree.txt");
    // printf("reading ended\n");


    tree_print(tree);
    printf("\n");

    Node* der1 = derivative(tree);

    printf("\n");
    tree_print(der1);
    getchar();
    printf("\n");
    simplify(der1);
    tree_print(der1);
    // simplify(der1);
    // printf("\n");
    // tree_print(der1);

    return 0;
}
