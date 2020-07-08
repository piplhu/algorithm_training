#include "rbtree.h"
#include "stdio.h"

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

void insert_node(rbtree* t, KEY key, uint32_t value){
    if(t->root == t->nil){
        t->root = create_red_node(key,value,t->nil);
        return;
    }

    rbtree_node* nil = t->nil;
    rbtree_node* x = t->root;
    while (x != nil){
        if(x->key > key){
            x = x->right;
        }else if(x->key < key){
            x = x->left;
        }else{ //==
            return;
        }
    }
    x = create_red_node(key,value,nil);
    
    
}

void print_node(rbtree_node* node,rbtree_node* nil){
    if(node != nil)
        printf("key = %d; color = %s",node->key,node->color == RED ? "RED" : "BLACK");
    print_node(node->left,nil);
    print_node(node->right,nil);
}

void rbtree_print(rbtree* t){
    rbtree_node* node = t->root;
    rbtree_node* nil = t->nil;
    if(node != nil){
        print_node(node,nil);       
    }
}
