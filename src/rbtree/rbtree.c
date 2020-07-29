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

void init_rbtree(rbtree **t) {
    *t              = (rbtree *)malloc(sizeof(rbtree));
    rbtree *tmp     = *t;
    tmp->nil        = (rbtree_node *)malloc(sizeof(rbtree_node));
    tmp->nil->color = BLACK;
    tmp->root       = tmp->nil;
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
    rbtree_node *right = node->right;
    node->right        = right->left;
    if (right->left != nil) right->left->parent = node;

    right->parent = node->parent;

    if (node->parent == nil) {
        (*root) = right;
    } else if (node == node->parent->left) {
        node->parent->left = right;
    } else {
        node->parent->right = right;
    }

    right->left  = node;
    node->parent = right;
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
    rbtree_node *left = node->left; //要旋转节点的左节点
    node->left = left->right; //将左节点的右子节点给要旋转的节点的左节点
    if (left->right != nil) //将左节点的右子节点的父指向要旋转的节点
        left->right->parent = node;

    left->parent = node->parent; //左节点的父节点变成旋转节点的父节点

    //更新旋转节点的父节点
    if (node->parent == nil) { //旋转节点是ROOT
        (*root) = left;
    } else if (node == node->parent->right) { //要旋转的节点的是父节点的右节点
        node->parent->right = left;
    } else { //要旋转的节点的是父节点的右节点
        node->parent->left = left;
    }
    left->right  = node;
    node->parent = left;
}

/**
 * @brief 删除自平衡
 * 
 * @param root 
 * @param nil 
 * @param node 插入的子节点
 */
void rbtree_delete_fixup(rbtree_node **root, rbtree_node *nil,
                         rbtree_node *node) {
    while (node != (*root) && node->color == BLACK) {
        if (node == node->parent->left) { //要插入的节点是父节点的左节点
            rbtree_node *right_br = node->parent->right; //右兄弟
            if (right_br->color == RED) {
                right_br->color     = BLACK;
                node->parent->color = RED;

                rbtree_left_rotate(root, nil, node->parent);
                right_br = node->parent->right;
            }

            if (right_br->left->color == BLACK &&
                right_br->right->color == BLACK) {
                right_br->color = RED;
                node            = node->parent;
            } else {
                if (right_br->right->color == BLACK) {
                    right_br->left->color = BLACK;
                    right_br->color       = RED;
                    rbtree_right_rotate(root, nil, right_br);
                    right_br = node->parent->right;
                }

                right_br->color        = node->parent->color;
                node->parent->color    = BLACK;
                right_br->right->color = BLACK;
                rbtree_left_rotate(root, nil, node->parent);

                node = *root;
            }
        } else {
            rbtree_node *left_rb = node->parent->left;
            if (left_rb->color == RED) {
                left_rb->color      = BLACK;
                node->parent->color = RED;
                rbtree_right_rotate(root, nil, node->parent);
                left_rb = node->parent->left;
            }

            if (left_rb->left->color == BLACK &&
                left_rb->right->color == BLACK) {
                left_rb->color = RED;
                node           = node->parent;
            } else {
                if (left_rb->left->color == BLACK) {
                    left_rb->right->color = BLACK;
                    left_rb->color        = RED;
                    rbtree_left_rotate(root, nil, left_rb);
                    left_rb = node->parent->left;
                }

                left_rb->color       = node->parent->color;
                node->parent->color  = BLACK;
                left_rb->left->color = BLACK;
                rbtree_right_rotate(root, nil, node->parent);

                node = *root;
            }
        }
    }
    node->color = BLACK;
}

/**
 * @brief 插入自平衡
 * 
 * @param root 
 * @param nil 
 * @param node 
 */
void rbtree_insert_fixup(rbtree_node **root, rbtree_node *nil,
                         rbtree_node *node) {
    while (node->parent->color == RED) {
        if (node->parent == node->parent->parent->left) {
            rbtree_node *right_uncle = node->parent->parent->right;
            if (right_uncle->color == RED) {
                node->parent->color         = BLACK;
                right_uncle->color          = BLACK;
                node->parent->parent->color = RED;

                node = node->parent->parent;
            } else { //没有叔父节点

                if (node == node->parent->right) {
                    node = node->parent;
                    rbtree_left_rotate(root, nil, node);
                }

                node->parent->color         = BLACK;
                node->parent->parent->color = RED;
                rbtree_right_rotate(root, nil, node->parent->parent);
            }
        } else {
            rbtree_node *left_uncle = node->parent->parent->left;
            if (left_uncle->color == RED) {
                node->parent->color         = BLACK;
                left_uncle->color           = BLACK;
                node->parent->parent->color = RED;

                node = node->parent->parent;
            } else {
                if (node == node->parent->left) {
                    node = node->parent;
                    rbtree_right_rotate(root, nil, node);
                }

                node->parent->color         = BLACK;
                node->parent->parent->color = RED;
                rbtree_left_rotate(root, nil, node->parent->parent);
            }
        }
    }

    (*root)->color = BLACK;
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
    rbtree_node* node = create_red_node(key,value,t->nil);

    node->parent = p;
    if(p == t->nil){
        t->root = node;
    }else if(p->key > key){
        p->left = node;
    }else{
        p->right = node;
    }
    
    rbtree_insert_fixup(&(t->root),t->nil,node);
}

/**
 * @brief 找node节点下的最小节点
 * 
 * @param nil 
 * @param node 
 * @return rbtree_node* 
 */
rbtree_node *rbtree_mini(rbtree_node *nil, rbtree_node *node) {
    while (node->left != nil)
        node = node->left;

    return node;
}

/**
 * @brief 找node节点下的最大节点
 * 
 * @param nil 
 * @param node 
 * @return rbtree_node* 
 */
rbtree_node *rbtree_maxi(rbtree_node *nil, rbtree_node *node) {
    while (node->right != nil)
        node = node->right;

    return node;
}

/**
 * @brief 找node节点的后继节点
 * 
 * @param nil 
 * @param node 
 * @return rbtree_node* 
 */
rbtree_node *rbtree_successor(rbtree_node *nil, rbtree_node *node) {
    rbtree_node *tmp = node->parent;

    if (node->right != nil) { // 向下
        return rbtree_mini(nil, node->right);
    }

    while (tmp != nil && node != tmp->right) {//向上
        node = tmp;
        tmp  = tmp->parent;
    }

    return tmp;
}

rbtree_node *rbtree_search(rbtree *t, KEY key) {
    rbtree_node *node = t->root;
    while (node != t->nil) {
        if (node->key < key) {
            node = node->right;
        } else if (node->key > key) {
            node = node->left;
        } else {
            return node;
        }
    }
    return t->nil;
}


rbtree_node *delete_node(rbtree *t, KEY key) {
    rbtree_node *node = rbtree_search(t, key);

    rbtree_node *x = t->nil;
    rbtree_node *y = t->nil;

    if (node->left == t->nil || node->right == t->nil) {
        y = node;
    } else {
        y = rbtree_successor(t->nil, node);
    }

    if (y->left != t->nil) {
        x = y->left;
    } else if (y->right != t->nil) {
        x = y->right;
    }

    x->parent = y->parent;
    if (y->parent == t->nil) {
        t->root = x;
    } else if (y == y->parent->left) {
        y->parent->left = x;
    } else {
        y->parent->right = x;
    }

    if (y != node) {
        node->key   = y->key;
        node->value = y->value;
    }

    if (y->color == BLACK) {
        rbtree_delete_fixup(&(t->root),t->nil,x);
    }

    return y;
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
