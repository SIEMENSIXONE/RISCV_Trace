//--------------------------------------------------------------------------
// Main
#include <stdio.h>
//
int sum(int a, int b){
    return (a+b);
}

int mul(int a, int b){
    int result = 0;
    for (int i = 0; i < b; i++){
        result = sum(result, a);
    }
    return result;
}

int main( int argc, char* argv[] )
{
    int a = 5;
    int b = 10;
    int c = 0;
    c = mul(a, b);
    printf("%d \n", c);
    return 0;
}