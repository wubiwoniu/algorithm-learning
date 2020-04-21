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
    DataType color;
    DataType data;    //节点附属信息
    g_edge first_arc; //该节点连出去的第一条边
};
struct g_list
{
    int v_num, e_num;
    adj_node adj;
};

typedef int ItemType;
typedef struct node *LNode;
typedef struct list *queue;
struct node
{
    ItemType data;
    LNode next;
};
struct list
{
    LNode head;
};

graph_list graph_create(int vertex_size);                      //创建一个顶点数vertex_size的邻接表
graph_list graph_insert(graph_list graph, Vertex u, Vertex v); //插入边(u,v);
graph_list graph_delete(graph_list graph, Vertex u, Vertex v); //删除边(u,v)
bool graph_search(graph_list graph, Vertex u, Vertex v);       //查询是否存在边(u,v)
void graph_show(graph_list graph);

queue create_queue();
bool is_empty(queue Q);
queue enqueue(queue Q, ItemType value);
ItemType dequeue(queue Q);

void bfs(graph_list graph, Vertex u);

int main()
{
    int array[] = {0, 1, 0, 3, 0, 4, 1, 4, 2, 3, 2, 1, 3, 4};
    graph_list graph = graph_create(5);
    for (int i = 0; i < 14; i += 2)
        graph = graph_insert(graph, array[i], array[i + 1]);
    graph_show(graph);
    bfs(graph, 0);
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

queue create_queue()
{
    queue Q = (queue)malloc(sizeof(struct list));
    Q->head = (LNode)malloc(sizeof(struct node));
    Q->head->next = NULL;
    return Q;
}

bool is_empty(queue Q)
{
    return Q->head->next == NULL ? true : false;
}

queue enqueue(queue Q, ItemType value)
{
    LNode node = (LNode)malloc(sizeof(struct node));
    node->data = value;
    LNode index = Q->head;
    while (index->next != NULL)
        index = index->next;
    node->next = index->next;
    index->next = node;
    return Q;
}

ItemType dequeue(queue Q)
{
    ItemType ans;
    LNode node = Q->head->next;
    if (node != NULL)
    {
        ans = node->data;
        Q->head->next = node->next;
        free(node);
        return ans;
    }
    return -1;
}

void bfs(graph_list graph, Vertex u)
{
    queue Q = create_queue();
    //0为没访问过相邻节点,当节点出队并访问过所有邻接点后则置1,表示不用再次访问
    for (int i = 0; i < graph->v_num; i++)
        graph->adj[i].color = 0;
    Q = enqueue(Q, u);
    //其实这段只能查一个连通分量(u所在的连通分量)
    while (!is_empty(Q))
    {
        Vertex v = dequeue(Q);
        g_edge temp = graph->adj[v].first_arc;
        while (temp != NULL)
        {
            //如果相邻的点没被找过,入队准备访问
            if (graph->adj[temp->v].color == 0)
            {
                Q = enqueue(Q, temp->v);
            }
            temp = temp->next;
        }
        printf("%d ", v);
        graph->adj[v].color = 1;
    }
    printf("\n");
}