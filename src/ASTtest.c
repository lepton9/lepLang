#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef enum {
    NODE_FUNCTION,
    NODE_PARAMETER,
    NODE_STATEMENT,
    NODE_VARIABLE_DECLARATION,
    NODE_ASSIGNMENT,
    NODE_RETURN,
    NODE_EXPRESSION,
    NODE_TYPE,
    NODE_IDENTIFIER,
    NODE_INTEGER,
    NODE_OPERATOR
} NodeType;

typedef struct ASTNode {
    NodeType type;
    struct ASTNode *left;
    struct ASTNode *right;
    char *value;
} ASTNode;

ASTNode* create_ast_node(NodeType type, char *value) {
    ASTNode *node = (ASTNode*)malloc(sizeof(ASTNode));
    node->type = type;
    node->value = value ? strdup(value) : NULL;
    node->left = NULL;
    node->right = NULL;
    return node;
}

void print_ast(ASTNode *node, int indent) {
    if (!node) return;

    for (int i = 0; i < indent; i++) printf("  ");

    switch (node->type) {
        case NODE_FUNCTION:
            printf("Function\n");
            break;
        case NODE_PARAMETER:
            printf("Parameter\n");
            break;
        case NODE_STATEMENT:
            printf("Statement\n");
            break;
        case NODE_VARIABLE_DECLARATION:
            printf("Variable Declaration\n");
            break;
        case NODE_ASSIGNMENT:
            printf("Assignment\n");
            break;
        case NODE_RETURN:
            printf("Return\n");
            break;
        case NODE_EXPRESSION:
            printf("Expression\n");
            break;
        case NODE_TYPE:
            printf("Type: %s\n", node->value);
            break;
        case NODE_IDENTIFIER:
            printf("Identifier: %s\n", node->value);
            break;
        case NODE_INTEGER:
            printf("Integer: %s\n", node->value);
            break;
        case NODE_OPERATOR:
            printf("Operator: %s\n", node->value);
            break;
    }

    print_ast(node->left, indent + 1);
    print_ast(node->right, indent + 1);
}

