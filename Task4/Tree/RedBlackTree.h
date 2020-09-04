#pragma once

enum RedBlackNodeColor { BLACK, RED };

class RedBlackNode
{
private:
    RedBlackNode();
public:
    RedBlackNode(RedBlackNode*, RedBlackNode*, RedBlackNode*, RedBlackNodeColor, int);
    ~RedBlackNode();

    RedBlackNode* left;         /* left child */
    RedBlackNode* right;        /* right child */
    RedBlackNode* parent;       /* parent */
    RedBlackNodeColor color;    /* node color (BLACK, RED) */
    int data;                   /* data stored in node */
};


class RedBlackTree
{
public:
    RedBlackTree();
    ~RedBlackTree();

    RedBlackNode* sentinel;
    RedBlackNode* root = sentinel;

    RedBlackNode* insertNode(int);
    void deleteNode(RedBlackNode*);
    RedBlackNode* findNode(int data);

private:
    void rotateLeft(RedBlackNode*);
    void rotateRight(RedBlackNode*);

    void insertFixup(RedBlackNode*);
    void deleteFixup(RedBlackNode*);
};

