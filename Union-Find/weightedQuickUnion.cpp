#include <iostream>
using namespace std;

class UF
{
private:
    int *id;
    int *size;
    int cnt;
    int id_length;

public:
    UF(int N);
    ~UF();
    int count();
    bool connected(int p, int q); //
    int find(int p);
    void Union(int p, int q);
};

UF::UF(int N)
{
    cnt = N;
    id_length = N;
    id = new int[N];
    size = new int[N]; //根节点所在分量的大小，只对根节点有效
    for (int i = 0; i < N; i++)
    {
        id[i] = i;
        size[i] = 1;
    }
}

UF::~UF()
{
    delete[] id;
}

int UF::count()
{
    return cnt;
}

bool UF::connected(int p, int q)
{
    return find(p) == find(q);
}

int UF::find(int p)
{
    while (p != id[p])
        p = id[p];
    return p;
}

void UF::Union(int p, int q)
{
    int pID = find(p);
    int qID = find(q);
    if (pID == qID)
        return;
    else if (pID > qID)
    {
        id[qID] = pID;
        size[pID] += size[qID];
    }
    else
    {
        id[pID] = qID;
        size[qID] += size[pID];
    }
    cnt--;
}

int main()
{
}
