#include "rbtree.h"

int main(){

    //init
    rbtree_node* nil = (rbtree_node*)malloc(sizeof(nil));
    nil->color = BLACK;
    rbtree* t = NULL;
    init_rbtree(&t,nil);

    //
    insert_node(t,10,20);
    insert_node(t,11,20);
    insert_node(t,9,20);
    insert_node(t,1,20);
    rbtree_print(t);
    return 0;
}