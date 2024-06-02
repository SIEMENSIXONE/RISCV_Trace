#include <stdio.h>

int mul(int a, int b){
    int result = 0;
    for (int i = 0; i < b; i++){
        result = result + a;
    }
    return result;
}

int power(int val, int exp){
    int result = 1;
    for (int i = 0; i < exp; i++){
        result = mul(result, val);
    }
    return result;
}

void printResult(int a){
    printf("%d\n", a);
}

int main(int argc, const char * argv[]) {
    int result = power(1024, 3);
    printResult(result);
    return 0;
}

