#include "header.h"
#include <stdio.h>

int main(int argc, const char * argv[]) {
    //myFunc
    printf("myFunc");
    //myFunc(int val){
    printf("myFunc(int val){");
    myFunc(5);
    return 0;
}

int myFunc(int val){
    printf("%d\n", val);
    return val;
}
