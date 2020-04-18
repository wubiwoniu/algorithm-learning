#include <iostream>
using namespace std;

class UF
{
private:
    int *id;
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
    for (int i = 0; i < N; i++)
        id[i] = i;
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
    return id[p];
}

void UF::Union(int p, int q)
{
    int pID = find(p);
    int qID = find(q);
    if (pID == qID)
        return;
    for (int i = 0; i < id_length; i++)
    {
        if (id[i] == qID)
            id[i] = pID;
    }
    cnt--;
}

int main()
{
}