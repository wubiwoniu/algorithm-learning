#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <algorithm>

typedef int Vertex;
typedef struct edge *Edge;
struct edge
{
    Vertex u, v;
    int w;
};
typedef struct edges *Edges;
struct edges
{
    Edge E;
    int length;
};

typedef struct union_find *Union;
struct union_find
{
    Vertex *id;
    int length;
};

Edges create_edges(int N);
Edges edge_insert(Edges G, Vertex u, Vertex v, int w);
int get_weight(Edges G);

Union create_union(int N);
Vertex find(Union U, Vertex u);
bool connected(Union U, Vertex u, Vertex v);
void edge_union(Union U, Vertex u, Vertex v);

bool cmp(edge a, edge b);

Edges kruskal(Edges G, Union U);

int main()
{
    //28
    int array[] = {
        0, 1, 4,
        0, 7, 8,
        1, 7, 11,
        1, 2, 8,
        8, 2, 2,
        8, 7, 7,
        8, 6, 6,
        7, 6, 1,
        2, 3, 7,
        2, 5, 4,
        6, 5, 2,
        3, 5, 14,
        3, 4, 9,
        4, 5, 10};
    Edges G = create_edges(28);
    Union U = create_union(9);
    for (int i = 0; i < 14 * 3; i += 3)
    {
        G = edge_insert(G, array[i], array[i + 1], array[i + 2]);
        //G = edge_insert(G, array[i + 1], array[i], array[i + 2]);
    }
    Edges ans = kruskal(G, U);
    printf("weight = %d\n", get_weight(ans));
    system("PAUSE");
}

Edges create_edges(int N)
{
    Edges G = (Edges)malloc(sizeof(struct edges));
    G->E = (Edge)malloc(sizeof(struct edge) * N);
    G->length = 0;
    return G;
}

Edges edge_insert(Edges G, Vertex u, Vertex v, int w)
{
    G->E[G->length].u = u;
    G->E[G->length].v = v;
    G->E[G->length].w = w;
    G->length++;
    return G;
}

int get_weight(Edges G)
{
    int ans = 0;
    for (int i = 0; i < G->length; i++)
        ans += G->E[i].w;
    return ans;
}

Union create_union(int N)
{
    Union U = (Union)malloc(sizeof(struct union_find));
    U->id = (Vertex *)malloc(sizeof(Vertex) * N);
    U->length = N;
    for (int i = 0; i < U->length; i++)
        U->id[i] = i;
    return U;
}

Vertex find(Union U, Vertex u)
{
    while (u != U->id[u])
        u = U->id[u];
    return u;
}

bool connected(Union U, Vertex u, Vertex v)
{
    return find(U, u) == find(U, v);
}

void edge_union(Union U, Vertex u, Vertex v)
{
    Vertex UId = find(U, u);
    Vertex VId = find(U, v);
    if (UId != VId)
        U->id[UId] = VId;
}

bool cmp(edge a, edge b)
{
    return a.w < b.w;
}

//kruskal的基本思想:将边按权重从小到大排序依次查看,如果当前边所连接的节点处于
//不同连通分量则加入
Edges kruskal(Edges G, Union U)
{
    int cnt = 0;
    Edges ans = create_edges(U->length - 1);
    std::sort(G->E, (G->E) + G->length, cmp);
    /*for (int i = 0; i < G->length; i++)
        printf("%d ", G->E[i].w);
    printf("\n");*/
    for (int i = 0; i < G->length; i++)
    {
        if (!connected(U, G->E[i].u, G->E[i].v))
        {
            edge_union(U, G->E[i].u, G->E[i].v);
            ans = edge_insert(ans, G->E[i].u, G->E[i].v, G->E[i].w);
            printf("%d %d %d\n", G->E[i].u, G->E[i].v, G->E[i].w);
            for (int i = 0; i < U->length; i++)
                printf("%d ", U->id[i]);
            printf("\n");
            cnt++;
        }
        //大于等于节点数-1
        if (cnt >= U->length - 1)
            break;
    }
    return ans;
}