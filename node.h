#ifndef NODE_H
#define NODE_H
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
typedef struct node {   // alias: Node
    int lineNo;         //  node in which line
    NodeType type;      // node type
    char *name;         //  node name
    char *val;          //  node value
    struct node *child; //  non-terminals node first child node
    struct node *next;  //  non-terminals node next brother node

} Node;

typedef Node *ptrNode;

ptrNode newNode(int lineNo, NodeType type, char *name, int argc, ...);

ptrNode newTokenNode(int lineNo, NodeType type, char *name, char *tokenText);

void deleteNode(ptrNode node);

void printTreeInfo(ptrNode node, int height);

#endif