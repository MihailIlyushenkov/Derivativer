#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tree.h"

int MAXSTRINGSIZE = 1000;
int MAXIDSIZE = 40;
const int DEFAULTVARIABLESCOUNT = 5;

const int OPCOUNTER = 8;
const char* operators[OPCOUNTER] = {"+", "-", "*", "/", "(", ")", "sin", "sqrt"};

const char* s;
int p;

Node* GetP();
Node* GetE();
Node* GetN();
Node* GetT();
Node* GetG(const char* string);

void synt_assert(int boolean) {
    if (boolean) {
        return;
    }
    else {
        printf("syntax error from char: %s", s+p);
        return;
    }
}

Node* GetN() {
    // printf("N\n");
    int p_old = p;
    double val = 0;
    while (('0' <= s[p]) && (s[p] <= '9')) {
        val = val*10 + (s[p] - '0');
        p++;
    }
    synt_assert(p > p_old);

    return create_node(val, value, nullptr, nullptr);
}

#define ISCHAR(x) ('a' <= (x) <= 'z')?1:('A' <= (x) <= 'Z')?1:0
#define ISDIGIT(x) ('0' <= (x) <= '9')?1:0


Node* GetP() {
    // printf("P\n");
    Node* val = 0;
    if (s[p] == '(') {
        // printf("readed (\n");
        p++;
        val = GetE();
        synt_assert(s[p] == ')');
        p++;
        return val;
    }
    else if (s[p] == 'x') {
        p++;
        return create_node(0, variable,  nullptr, nullptr);

    }
    else {
        // printf("not readed (\n");
        val = GetN();
        return val;
    }
}

Node* GetT () {
    // printf("T\n");

    Node* val = GetP();
    while ((s[p] == '*') || (s[p] == '/')) {
        int op = s[p];
        p++;
        Node* val2 = GetP();
        switch(op) {
            case '*': val = create_node(symb_operation_to_int('*'), oper, val, val2); break;
            case '/': val = create_node(symb_operation_to_int('/'), oper, val, val2); break;
            default: synt_assert(0); break;
        }
    }
    return val;
}

Node* GetE () {
    // printf("E\n");

    Node* val = GetT();
    while ((s[p] == '+') || (s[p] == '-')) {
        int op = s[p];
        p++;
        Node* val2 = GetT();
        switch(op) {
            case '+': val = create_node(symb_operation_to_int('+'), oper, val, val2); break;
            case '-': val = create_node(symb_operation_to_int('-'), oper, val, val2);; break;
            default: synt_assert(0); break;
        }
    }
    return val;
}

Node* GetG(const char* string) {
    // printf("G\n");
    s = string;
    p = 0;
    Node* val = GetE();
    synt_assert(s[p] == '\0');
    return val;
}

char* readbuf(size_t maxreaded, const char* filename) {
    FILE* datafile = fopen(filename, "r");
    if (!datafile) {
        printf("filenotopen\n");
        return 0;
    }

    char* buf = (char*) calloc(maxreaded, sizeof(char));

    int c = fgetc(datafile);
    for (int i = 0; (c > 0) && (i < maxreaded) && (c != '\n'); i++) {
        buf[i] = c;
        c = fgetc(datafile);
    }

    fclose(datafile);
    return buf;
}

// int compare_with_operation(int expression_pointer, int expression, int operation_number) {
//     int i = 0;
//
//     while (expression[expression_pointer + i] == operators[operation_number][i]) {
//         i++;
//     }
//
//     if (operators[operation_number][i] == '\0') {
//         return i;
//     }
//     else return 0;
// }
//
// lexem* lexical_analysis(const char* expression) {
//     lexem* lexems = calloc(MAXSTRINGSIZE, sizeof(lexem));
//     int lexval = 0;
//     int lexp = 0;
//     int i = 0;
//
//     for (; expression[i] != 0; i++) {
//         lexval = expression[i];
//         if (ISCHAR(lexval)) {
//             lexems[lexp].type = variable;
//             lexems[lexp].value = lexval;
//             i++;
//         }
//         else if (ISDIGIT(lexval)) {
//             while(ISDIGIT(expression[i])) {
//                 lexval = lexval * 10 + expression[i];
//                 i++;
//             }
//             lexems[lexp].type = value;
//             lexems[lexp].value = lexval;
//         }
//         else {
//             int shift = 0;
//             for(int j = 0; j < OPCOUNTER; j++) {
//                 shift = compare_with_operation(i, expression, j);
//                 if (shift != 0) {
//                     lexems[lexp].type = oper;
//                     lexems[lexp].value = j;
//                     i += j;
//                     break;
//                 }
//             }
//
//             if (shift == 0) synt_assert(i);
//
//         }
//     }
// }

// int main() {
//     char* buf = readbuf(MAXSTRINGSIZE, "Tree.txt");
//
//     // char buf[] = "1000-7*100/(30+5*10-5*(100/50))+1";
//     printf("%s\n", buf);
//
//
//     Node* x = GetG(buf);
//     // printf("%lf", x);
//     tree_print(x);
//
//     return 0;
// }
