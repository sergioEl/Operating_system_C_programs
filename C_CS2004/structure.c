// lecture3

#include <stdio.h>
#include <math.h>

struct point
{
    int x;
    int y;
    struct point *nextpoint; // list
};

struct rect
{
    struct point p1;
    struct point p2;
};


struct point makepoint(int x, int y)
{
    struct point temp;
    temp.x = x;
    temp.y = y;
    temp.nextpoint = 0;
    return temp;
}

int main()
{
    struct point mp1, mp2;
    struct point *i;
    mp1 = makepoint(5, 6);
    mp2 = makepoint(7, 8);
    mp1.nextpoint = &mp2;
    printf("%d %d\n", mp1.(*nextpoint), mp2.(*nextpoint);

    return 0;
}
