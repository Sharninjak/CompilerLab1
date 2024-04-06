#ifndef NODE_H
#define NODE_H

// #include <unistd.h>
#include <assert.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// #define NAME_LENGTH 32
// #define VAL_LENGTH 64

#define TRUE 1
#define FALSE 0

// define node type
typedef enum nodeType {
    TOKEN_INT,
    TOKEN_FLOAT,
    TOKEN_ID,
    TOKEN_TYPE,
    TOKEN_OTHER,
    NOT_A_TOKEN

} NodeType;

// typedef uint_32 bool;

// node type declared
typedef struct node { // alias: Node
    int lineNo;       //  node in which line
    //   int depth;   //  node depth, for count white space for print
    NodeType type; // node type
    char *name;    //  node name
    char *val;     //  node value

    struct node *child; //  non-terminals node first child node
    struct node *next;  //  non-terminals node next brother node

} Node;

typedef Node *ptrNode;

ptrNode newNode(int lineNo, NodeType type, char *name, int argc, ...) {
    ptrNode curNode = NULL;
    // int nameLength = strlen(name) + 1;
    // 指针自身 = (指针类型*）malloc（sizeof（指针类型）*数据数量）
    curNode = (Node *)malloc(sizeof(Node));
    // curNode = (ptrNode)malloc(sizeof(Node)); // 两个都行

    assert(curNode != NULL);

    // curNode->name = (char*)malloc(sizeof(char) * NAME_LENGTH);
    // curNode->val = (char*)malloc(sizeof(char) * VAL_LENGTH);
    // curNode->name = (char *)malloc(sizeof(char) * nameLength);
    // assert(curNode->name != NULL);
    // assert(curNode->val != NULL);

    curNode->lineNo = lineNo;
    curNode->type = type;
    curNode->name = name;
    // strncpy(curNode->name, name, nameLength);

    va_list
        vaList; // 存储可变参数列表信息的数据类型，是一个指向栈空间中某个位置的指针
    va_start(vaList, argc);

    ptrNode tempNode = va_arg(vaList, ptrNode);

    curNode->child = tempNode;

    for (int i = 1; i < argc; i++) {
        tempNode->next = va_arg(vaList, ptrNode);
        if (tempNode->next != NULL) {
            tempNode = tempNode->next;
        }
    }

    va_end(vaList);
    return curNode;
}

ptrNode newTokenNode(int lineNo, NodeType type, char *tokenName,
                     char *tokenText) {
    ptrNode tokenNode = (ptrNode)malloc(sizeof(Node));
    int nameLength = strlen(tokenName) + 1;
    int textLength = strlen(tokenText) + 1;

    assert(tokenNode != NULL);

    tokenNode->lineNo = lineNo;
    tokenNode->type = type;

    tokenNode->name = (char *)malloc(sizeof(char) * nameLength);
    tokenNode->val = (char *)malloc(sizeof(char) * textLength);

    // assert(tokenNode->name != NULL);
    // assert(tokenNode->val != NULL);
    tokenNode->name = tokenName;
    tokenNode->val = tokenText;
    // strncpy(tokenNode->name, tokenName, nameLength);
    // strncpy(tokenNode->val, tokenText, textLength);

    tokenNode->child = NULL;
    tokenNode->next = NULL;

    return tokenNode;
}

void delNode(ptrNode node) {
    if (node == NULL)
        return;
    while (node->child != NULL) {
        ptrNode temp = node->child;
        node->child = node->child->next;
        delNode(temp);
    }
    free(node->name);
    free(node->val);
    free(node);
    node->name = NULL;
    node->val = NULL;
    node = NULL;
}

void printTreeInfo(ptrNode curNode, int height) {
    if (curNode == NULL) {
        return;
    }

    for (int i = 0; i < height; i++) {
        printf("  ");
    }
    printf("%s", curNode->name);
    if (curNode->type == NOT_A_TOKEN) {
        printf(" (%d)", curNode->lineNo);
    } else if (curNode->type == TOKEN_TYPE || curNode->type == TOKEN_ID ||
               curNode->type == TOKEN_INT) {
        printf(": %s", curNode->val);
    } else if (curNode->type == TOKEN_FLOAT) {
        printf(": %lf", atof(curNode->val));
    }
    printf("\n");
    printTreeInfo(curNode->child, height + 1);
    printTreeInfo(curNode->next, height);
}

#endif