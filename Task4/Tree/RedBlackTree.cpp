#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

#include "RedBlackTree.h"

//////////////////////////////////////////////////
//////////////          NODE            //////////
//////////////////////////////////////////////////

RedBlackNode::RedBlackNode(RedBlackNode* _left, RedBlackNode* _right, RedBlackNode* _parent, RedBlackNodeColor _color,
int _data ) : left(_left), right(_right), parent(_parent), color(_color), data(_data)
{}

RedBlackNode::~RedBlackNode() {}

//////////////////////////////////////////////////
//////////////          TREE            //////////
//////////////////////////////////////////////////


RedBlackTree::RedBlackTree()
{
    sentinel = new RedBlackNode(nullptr, nullptr, 0, BLACK, 0);
    sentinel->left = sentinel;
    sentinel->right = sentinel;         /* all leafs are sentinels */

    root = sentinel;                    /* root of Red-Black tree */
}

RedBlackTree::~RedBlackTree()
{
    delete sentinel;
}

//#define compLT(a,b) (a < b)
//#define compEQ(a,b) (a == b)

void RedBlackTree::rotateLeft(RedBlackNode* x) {

    /**************************
     *  rotate node x to left *
     **************************/

    RedBlackNode* y = x->right;

    /* establish x->right link */
    x->right = y->left;
    if (y->left != sentinel) y->left->parent = x;

    /* establish y->parent link */
    if (y != sentinel) y->parent = x->parent;
    if (x->parent) {
        if (x == x->parent->left)
            x->parent->left = y;
        else
            x->parent->right = y;
    }
    else {
        root = y;
    }

    /* link x and y */
    y->left = x;
    if (x != sentinel) x->parent = y;
}

void RedBlackTree::rotateRight(RedBlackNode* x) {

    /****************************
     *  rotate node x to right  *
     ****************************/

    RedBlackNode* y = x->left;

    /* establish x->left link */
    x->left = y->right;
    if (y->right != sentinel) y->right->parent = x;

    /* establish y->parent link */
    if (y != sentinel) y->parent = x->parent;
    if (x->parent) {
        if (x == x->parent->right)
            x->parent->right = y;
        else
            x->parent->left = y;
    }
    else {
        root = y;
    }

    /* link x and y */
    y->right = x;
    if (x != sentinel) x->parent = y;
}

void RedBlackTree::insertFixup(RedBlackNode* x) {

    /*************************************
     *  maintain Red-Black tree balance  *
     *  after inserting node x           *
     *************************************/

     /* check Red-Black properties */
    while (x != root && x->parent->color == RED) {
        /* we have a violation */
        if (x->parent == x->parent->parent->left) {
            RedBlackNode* y = x->parent->parent->right;
            if (y->color == RED) {

                /* uncle is RED */
                x->parent->color = BLACK;
                y->color = BLACK;
                x->parent->parent->color = RED;
                x = x->parent->parent;
            }
            else {

                /* uncle is BLACK */
                if (x == x->parent->right) {
                    /* make x a left child */
                    x = x->parent;
                    rotateLeft(x);
                }

                /* recolor and rotate */
                x->parent->color = BLACK;
                x->parent->parent->color = RED;
                rotateRight(x->parent->parent);
            }
        }
        else {

            /* mirror image of above code */
            RedBlackNode* y = x->parent->parent->left;
            if (y->color == RED) {

                /* uncle is RED */
                x->parent->color = BLACK;
                y->color = BLACK;
                x->parent->parent->color = RED;
                x = x->parent->parent;
            }
            else {

                /* uncle is BLACK */
                if (x == x->parent->left) {
                    x = x->parent;
                    rotateRight(x);
                }
                x->parent->color = BLACK;
                x->parent->parent->color = RED;
                rotateLeft(x->parent->parent);
            }
        }
    }
    root->color = BLACK;
}

RedBlackNode* RedBlackTree::insertNode(int data) {
    RedBlackNode* current, * parent, * x;

    /***********************************************
     *  allocate node for data and insert in tree  *
     ***********************************************/

     /* find where node belongs */
    current = root;
    parent = 0;
    while (current != sentinel) {
        if (data == current->data) return (current);
        parent = current;
        current = (data < current->data) ?
            current->left : current->right;
    }

    /* setup new node */
    x = new RedBlackNode(sentinel, sentinel, parent, RED, data);
    if (x == nullptr) 
    {
        printf("insufficient memory (insertNode)\n");
        exit(1);
    }

    /* insert node in tree */
    if (parent) {
        if (data < parent->data)
            parent->left = x;
        else
            parent->right = x;
    }
    else {
        root = x;
    }

    insertFixup(x);
    return(x);
}

void RedBlackTree::deleteFixup(RedBlackNode* x) {

    /*************************************
     *  maintain Red-Black tree balance  *
     *  after deleting node x            *
     *************************************/

    while (x != root && x->color == BLACK) {
        if (x == x->parent->left) {
            RedBlackNode* w = x->parent->right;
            if (w->color == RED) {
                w->color = BLACK;
                x->parent->color = RED;
                rotateLeft(x->parent);
                w = x->parent->right;
            }
            if (w->left->color == BLACK && w->right->color == BLACK) {
                w->color = RED;
                x = x->parent;
            }
            else {
                if (w->right->color == BLACK) {
                    w->left->color = BLACK;
                    w->color = RED;
                    rotateRight(w);
                    w = x->parent->right;
                }
                w->color = x->parent->color;
                x->parent->color = BLACK;
                w->right->color = BLACK;
                rotateLeft(x->parent);
                x = root;
            }
        }
        else {
            RedBlackNode* w = x->parent->left;
            if (w->color == RED) {
                w->color = BLACK;
                x->parent->color = RED;
                rotateRight(x->parent);
                w = x->parent->left;
            }
            if (w->right->color == BLACK && w->left->color == BLACK) {
                w->color = RED;
                x = x->parent;
            }
            else {
                if (w->left->color == BLACK) {
                    w->right->color = BLACK;
                    w->color = RED;
                    rotateLeft(w);
                    w = x->parent->left;
                }
                w->color = x->parent->color;
                x->parent->color = BLACK;
                w->left->color = BLACK;
                rotateRight(x->parent);
                x = root;
            }
        }
    }
    x->color = BLACK;
}

void RedBlackTree::deleteNode(RedBlackNode* z) {
    RedBlackNode* x, * y;

    /*****************************
     *  delete node z from tree  *
     *****************************/

    if (!z || z == sentinel) return;


    if (z->left == sentinel || z->right == sentinel) {
        /* y has a NIL node as a child */
        y = z;
    }
    else {
        /* find tree successor with a NIL node as a child */
        y = z->right;
        while (y->left != sentinel) y = y->left;
    }

    /* x is y's only child */
    if (y->left != sentinel)
        x = y->left;
    else
        x = y->right;

    /* remove y from the parent chain */
    x->parent = y->parent;
    if (y->parent)
        if (y == y->parent->left)
            y->parent->left = x;
        else
            y->parent->right = x;
    else
        root = x;

    if (y != z) z->data = y->data;


    if (y->color == BLACK)
        deleteFixup(x);

    delete y;
}

RedBlackNode* RedBlackTree::findNode(int data) {

    /*******************************
     *  find node containing data  *
     *******************************/

    RedBlackNode* current = root;
    while (current != sentinel)
        if (data == current->data)
            return (current);
        else
            current = (data < current->data) ?
            current->left : current->right;
    return(0);
}