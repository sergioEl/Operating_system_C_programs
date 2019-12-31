// lecture3

#include <stdio.h>
#include <math.h>

int main() {
    int a[6] = {1,2,3,4,5,6};
    int *pa = a;
    for (int i = 0; i < 6; i++)
    {
        printf("%d %d\n", i, *pa);
        pa++;
    }
    

    return 0;
}