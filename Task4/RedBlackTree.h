#pragma once

#include <vector>
#include "Employee.h"

enum RedBlackNodeColor { BLACK, RED };

class RedBlackNode
{
private:
    RedBlackNode();
public:
    RedBlackNode(RedBlackNode*, RedBlackNode*, RedBlackNode*, RedBlackNodeColor, Employee*);
    ~RedBlackNode();

    RedBlackNode* left;         /* left child */
    RedBlackNode* right;        /* right child */
    RedBlackNode* parent;       /* parent */
    RedBlackNodeColor color;    /* node color (BLACK, RED) */
    Employee* data;                   /* data stored in node */
};


class RedBlackTree
{
public:
    RedBlackTree();
    ~RedBlackTree();

    RedBlackNode* sentinel;
    RedBlackNode* root = sentinel;

    RedBlackNode* insertNode(Employee*);
    void deleteNode(RedBlackNode*);
    RedBlackNode* findNode(Employee* data);

    /////   More
    int costMax();
    int longMax();

private:
    void rotateLeft(RedBlackNode*);
    void rotateRight(RedBlackNode*);

    void insertFixup(RedBlackNode*);
    void deleteFixup(RedBlackNode*);

    void DeleteTree(RedBlackNode*);

    /////   More
    void costMax_R(RedBlackNode*, int&);
    void longMax_R(RedBlackNode*, int&);
};

