#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef int Vertex;
typedef int DataType;

//邻接表
typedef struct edge *g_edge;
typedef struct vertex_node *adj_node;
typedef struct g_list *graph_list;
struct edge
{
    Vertex v; //边中只存储(u,v)中的v,因为邻接表要找到(u,v)必定要先找到u
    g_edge next;
};
struct vertex_node
{
    int visit;
    DataType data;    //节点附属信息
    g_edge first_arc; //该节点连出去的第一条边
};
struct g_list
{
    int v_num, e_num;
    adj_node adj;
};

graph_list graph_create(int vertex_size);                      //创建一个顶点数vertex_size的邻接表
graph_list graph_insert(graph_list graph, Vertex u, Vertex v); //插入边(u,v);
graph_list graph_delete(graph_list graph, Vertex u, Vertex v); //删除边(u,v)
bool graph_search(graph_list graph, Vertex u, Vertex v);       //查询是否存在边(u,v)
void graph_show(graph_list graph);

void dfs(graph_list graph, Vertex u);

int main()
{
    int array[] = {0, 1, 1, 3, 3, 2, 2, 4};
    graph_list graph = graph_create(5);
    for (int i = 0; i < 8; i += 2)
        graph = graph_insert(graph, array[i], array[i + 1]);
    graph_show(graph);
    dfs(graph, 0);
    system("PAUSE");
}

graph_list graph_create(int vertex_size)
{
    graph_list graph = (graph_list)malloc(sizeof(struct g_list));
    graph->v_num = vertex_size;
    graph->e_num = 0;
    graph->adj = (adj_node)malloc(sizeof(struct vertex_node) * vertex_size);
    for (int i = 0; i < vertex_size; i++)
    {
        graph->adj[i].visit = 0; //全设为未访问
        graph->adj[i].data = i;
        graph->adj[i].first_arc = NULL;
    }
    return graph;
}

graph_list graph_insert(graph_list graph, Vertex u, Vertex v)
{
    g_edge new_edge = (g_edge)malloc(sizeof(struct edge));
    new_edge->v = v;
    new_edge->next = graph->adj[u].first_arc;
    graph->adj[u].first_arc = new_edge;
    graph->e_num++;
    return graph;
}

void graph_show(graph_list graph)
{
    for (int i = 0; i < graph->v_num; i++)
    {
        printf("%d ", i);
        g_edge temp = graph->adj[i].first_arc;
        while (temp != NULL)
        {
            printf("%d ", temp->v);
            temp = temp->next;
        }
        printf("\n");
    }
}

//从节点u开始dfs
void dfs(graph_list graph, Vertex u)
{
    printf("%d ", u);
    g_edge edge = graph->adj[u].first_arc;
    //查看所有邻接点
    while (edge != NULL)
    {
        //如果该点还未访问,则对该点进行dfs,当该点所有邻接点访问完时返回,此时设为已访问
        if (graph->adj[edge->v].visit == 0)
        {
            dfs(graph, edge->v);
        }
        graph->adj[edge->v].visit = 1;
        edge = edge->next;
    }
}