// lecture3

#include <stdio.h>
#include <math.h>

void swap(int *a, int *b){
    int temp;
    temp = *a;
    *a = *b;
    *b = temp;
}



int main() {
    int a = 2;
    int b = 16;
    printf("%d  %d\n", a, b);
    swap(&a, &b);
    printf("%d  %d\n", a, b);
    
    return 0;
}   