#include "sort.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define ARRAYNUM 20

void CreateData(int * array,int size){
     for(int i =0 ;i<size;i++){
        array[i]=rand()%100;
    } 
}

int main(){
    int array[ARRAYNUM]={0};
    srand(time(NULL));
/////////////////MPSort/////////////////////////////
    CreateData(array, ARRAYNUM);
    printf("MPSort before \n");
    Prinf(array, ARRAYNUM);
    MPSort(array, ARRAYNUM);
    printf("MPSort end \n");
    Prinf(array, ARRAYNUM);

//////////////////QSort////////////////////////////////////
    CreateData(array, ARRAYNUM);
    printf("QSort before \n");
    Prinf(array, ARRAYNUM);
    QSort(array, ARRAYNUM);
    printf("QSort end \n");
    Prinf(array, ARRAYNUM);

    return 0;
}
