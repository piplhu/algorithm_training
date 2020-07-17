/**
 * @file rbtree.c
 * @author pipl (huhui@hupipl.cn)
 * @brief 
 * @version 0.1
 * @date 2020-07-10
 * 
 * @copyright Copyright (c) 2020
 * 
 */

#include "rbtree.h"
#include "stdio.h"

/**
 * @brief Create a node object
 * 
 * @param key  
 * @param value 
 * @param color 
 * @param nil   :哨兵
 * @return rbtree_node* 
 */
rbtree_node* create_node(KEY key,int32_t value,char color,rbtree_node* nil){
    rbtree_node* node = (rbtree_node*)malloc(sizeof(rbtree_node));
    node->color = color;
    node->key = key;
    node->value = value;
    node->left = nil;
    node->right = nil;
    node->parent = nil;
    return node;
}


rbtree_node* create_black_node(KEY key,int32_t value,rbtree_node* nil){
    return create_node(key, value, BLACK, nil);
}

rbtree_node* create_red_node(KEY key,int32_t value,rbtree_node* nil){
    return create_node(key, value, RED, nil);
}

void init_rbtree(rbtree** t,rbtree_node* nil){
    *t = (rbtree*)malloc(sizeof(rbtree));
    rbtree* tmp = *t;
    tmp->root = nil;
    tmp->nil = nil;
}

/**
 * @brief 节点左旋
 * 
 * @param root 父节点
 * @param nil   哨兵
 * @param node  要旋转的节点
 */
void rbtree_left_rotate(rbtree_node **root, rbtree_node *nil,
                        rbtree_node *node) {

}

/**
 * @brief 节点右旋
 * 
 * @param root 父节点
 * @param nil  哨兵
 * @param node 要旋转的节点
 */
void rbtree_right_rotate(rbtree_node **root, rbtree_node *nil,
                         rbtree_node *node) {
}


void insert_node(rbtree* t, KEY key, uint32_t value){
    
    rbtree_node* p = t->nil;
    rbtree_node* x = t->root;
    while (x != t->nil){
        p = x;
        if(x->key < key){
            x = x->right;
        }else if(x->key > key){
            x = x->left;
        }else{ //==
            return;
        }
    }
    x = create_red_node(key,value,t->nil);
    if(p == t->nil){
        t->root = x;
        return;
    }

    x->parent = p;
    if(p->key > key){
        p->left = x;
    }else{
        p->right = x;
    }
    
    
}



void print_node(rbtree_node* node,rbtree_node* nil){
    if(node == nil)
        return;
    
    print_node(node->left,nil);
    printf("key = %d; value = %d; color = %s \n",node->key, node->value, node->color == RED ? "RED" : "BLACK");
    print_node(node->right,nil);
}

void rbtree_print(rbtree* t){
    rbtree_node* node = t->root;
    rbtree_node* nil = t->nil;
    if(node != nil){
        print_node(node,nil);       
    }
}
