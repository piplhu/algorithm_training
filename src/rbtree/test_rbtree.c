#include "rbtree.h"



int main(){

    int array[20]={24,25,13,35,23, 26,67,47,38,98, 20,19,17,49,12, 21,9,18,14,15};
    //init
    rbtree* t = NULL;
    init_rbtree(&t);

    //
    for(int i =0 ; i< 20 ;i++){
        insert_node(t,array[i],20);
    }
    rbtree_print(t);
    return 0;
}