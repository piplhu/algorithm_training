#ifndef _H_RBTREE_H_
#define _H_RBTREE_H_

#include <stdint.h>
#include <stdlib.h>

#define RED 1
#define BLACK 2

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

/**
 * @brief 初始化树，内部会为t分配空间
 * 
 * @param t 
 */
void init_rbtree(rbtree** t);

/**
 * @brief 插入节点
 * 
 * @param t 
 * @param key 
 * @param value 
 */
void insert_node(rbtree* t, KEY key, uint32_t value);

/**
 * @brief 删除节点
 * 
 * @param t 
 * @param key 
 * @return rbtree_node* 返回节点 
 */
rbtree_node* delete_node(rbtree* t, KEY key);

/**
 * @brief 查找Key对应的节点
 * 
 * @param t 
 * @param key 
 * @return rbtree_node* 
 */
rbtree_node* rbtree_search(rbtree* t,KEY key);

/**
 * @brief 中序遍历打印
 * 
 * @param t 
 */
void rbtree_print(rbtree* t);

#endif //_H_RBTREE_H_
