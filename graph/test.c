#include <stdio.h>
#include <stdlib.h>

int main()
{
    int *p;
    p = (int *)malloc(sizeof(int) * 2);
    p[0] = 1;
    p[1] = 2;
    printf("p0 = %d , p1 = %d\n", p[0], p[1]);
    system("PAUSE");
}