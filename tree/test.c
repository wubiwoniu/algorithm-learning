#include <stdio.h>
#include <stdlib.h>
struct s{
    int a;
    int b;
};

void SWAP(struct s *a)
{
    //a=(int*)malloc(sizeof(int));
    a->a=10;
    //return a;
}

int main()
{
    struct s *a;
    a=(struct s*)malloc(sizeof(struct s));
    a->a=5;
    SWAP(a);
    printf("%d\n",a->a);
    system("PAUSE");
}