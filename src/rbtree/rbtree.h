#ifndef _H_RBTREE_H_
#define _H_RBTREE_H_

#include <stdint.h>
#include <stdlib.h>

#define RED 0
#define BLACK 1

typedef unsigned int        KEY;
typedef struct _rbtree_node rbtree_node;
typedef struct _rbtree      rbtree;

struct _rbtree_node {
    KEY     key;
    char    color;
    int32_t value;

    rbtree_node* left;
    rbtree_node* right;
    rbtree_node* parent;
};

struct _rbtree {
    rbtree_node* root;
    rbtree_node* nil;
};

void init_rbtree(rbtree** t,rbtree_node* nil);
void insert_node(rbtree* t, KEY key, uint32_t value);
void rbtree_print(rbtree* t);
#endif //_H_RBTREE_H_
