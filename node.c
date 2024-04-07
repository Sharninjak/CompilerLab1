#include "node.h"
#include <assert.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
name: root's name
argc: num of para
*/
ptrNode newNode(int lineNo, NodeType type, char *name, int argc, ...) {
    ptrNode curNode = NULL;
    // 指针自身 = (指针类型*）malloc（sizeof（指针类型）*数据数量）
    curNode = (Node *)malloc(sizeof(Node)); // root
    // curNode = (ptrNode)malloc(sizeof(Node)); // 两个都行

    curNode->lineNo = lineNo;
    curNode->type = type;
    int nameLen = strlen(name) + 1; // include '\0'
    curNode->name =
        (char *)malloc(sizeof(char) * nameLen); // the byte name needs
    strncpy(curNode->name, name, nameLen);
    // curNode->name = name; // WRONG

    va_list
        vaList; // 存储可变参数列表信息的数据类型，是一个指向栈空间中某个位置的指针
    va_start(vaList, argc); // 初始化va_list类型的变量
    // va_arg获取可变参数列表中的下一个参数。修改va_list指针，使其指向参数列表中的下一个参数，并返回当前指向的参数值
    ptrNode tempNode = va_arg(vaList, ptrNode);

    curNode->child = tempNode;

    for (int i = 1; i < argc; i++) {
        ptrNode nextptr = va_arg(vaList, ptrNode);
        if (nextptr == NULL) {
            tempNode->next == NULL;
        } else {
            tempNode->next = nextptr;
            tempNode = tempNode->next;
        }
    }
    va_end(vaList); // 清理 va_list
    return curNode;
}

ptrNode newTokenNode(int lineNo, NodeType type, char *name, char *tokenText) {
    ptrNode tokenNode = (ptrNode)malloc(sizeof(Node));
    // assert(tokenNode != NULL);
    tokenNode->lineNo = lineNo;
    tokenNode->type = type;
    // tokenNode->name = name;
    // tokenNode->val = tokenText;
    int nameLength = strlen(name) + 1;
    int textLength = strlen(tokenText) + 1;
    // assert(tokenNode->name != NULL);
    // assert(tokenNode->val != NULL);
    tokenNode->name = (char *)malloc(sizeof(char) * nameLength);
    tokenNode->val = (char *)malloc(sizeof(char) * textLength);
    strncpy(tokenNode->name, name, nameLength);
    strncpy(tokenNode->val, tokenText, textLength);

    tokenNode->child = NULL;
    tokenNode->next = NULL;

    return tokenNode;
}

void deleteNode(ptrNode node) {
    if (node == NULL)
        return;
    ptrNode currentChild = node->child;
    ptrNode currentNext = node->next;
    deleteNode(currentChild);
    deleteNode(currentNext);
    free(node->name);
    free(node->val);
    free(node);
}

void printTreeInfo(ptrNode node, int height) {
    if (node == NULL) {
        return;
    }

    for (int i = 0; i < height; i++) {
        printf("  ");
    }
    printf("%s", node->name);
    switch (node->type) {
    case NOT_A_TOKEN:
        printf(" (%d)", node->lineNo);
        break;
    case TOKEN_TYPE:
    case TOKEN_ID:
    case TOKEN_INT:
        printf(": %s", node->val);
        break;
    case TOKEN_FLOAT:
        // 假设 val 是以字符串形式存储的浮点数
        printf(": %f", atof(node->val));
        break;
    // 可以继续添加其他类型的处理
    default:
        // 如果需要，可以打印未知类型的警告信息
        break;
    }
    printf("\n");
    printTreeInfo(node->child, height + 1);
    printTreeInfo(node->next, height);
}