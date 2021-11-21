#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>
#include<malloc.h>
#include<math.h>

#define inf 21474
#define  N 10000
#define minN 5050

struct fib_node {
    int dist;
    int vertex;
    int degree;
    int id;
    struct fib_node *par;
    struct fib_node *child;
    struct fib_node *left;
    struct fib_node *right;
    struct fib_node *x;
};

struct fib_node * fib_arr[minN];
struct fib_node * fib_root;

struct fib_node * init_fib_node(int distance, int vertex)
{
    struct fib_node * temp;
    temp = (struct fib_node*)malloc(sizeof (struct fib_node));

    temp->child = NULL;
    temp->par = NULL;
    temp->left = NULL;
    temp->right = NULL;
    temp->x = NULL;
    temp->degree = 0;
    temp->vertex = vertex;
    temp->dist = distance;
    return temp;
}




struct fib_node * fib_left_sibling(struct fib_node *temp)
{
    if (temp == NULL)return temp;
    while (temp->left != NULL) {
        temp = temp->left;
    }
    return temp;
}


struct fib_node * fib_right_sibling(struct fib_node *temp)
{
    if (temp == NULL)return temp;
    while (temp->right != NULL) {
        temp = temp->right;
    }
    return temp;
}



bool fib_add_sibling(struct fib_node *destined, struct fib_node * new_node)
{
    struct fib_node * temp;
    temp = fib_right_sibling(destined);

    if (temp == NULL)return false;

    temp->right = new_node;
    new_node->left = temp;
    new_node->par = destined->par;
    new_node->right = NULL;

    if (destined->par != NULL)(destined->par)->degree = (destined->par)->degree + 1;

    return true;
}



bool fib_add_child(struct fib_node *destined, struct fib_node *new_node)
{
    if (destined == NULL)return false;

    if (destined->child != NULL) {
        fib_add_sibling(destined->child, new_node);
    }
    else
    {
        destined->child = new_node;
        destined->degree = 1;
        new_node->par = destined;
    }
    return true;
}

bool fib_isEmpty()
{
    if (fib_root == NULL)return true;
    else
        return false;
}


struct fib_node * fib_push(int distance, int vertex)
{
    struct fib_node * temp;
    temp = init_fib_node(distance, vertex);

    if (fib_root == NULL) {
        fib_root = temp;
        return temp;
    }

    fib_add_sibling(fib_root, temp);

    if (fib_root->dist > temp->dist)fib_root = temp;

    return temp;
}


bool fib_cut_remove(struct fib_node * destined)
{
    if (destined->par != NULL)
    {
        ((destined->par)->degree) = ((destined->par)->degree) - 1;
        if (destined->left != NULL) {
            ((destined)->par)->child = destined->left;
        }
        else if (destined->right != NULL)
        {
            ((destined)->par)->child = destined->right;
        }
        else
        {
            destined->par->child = NULL;
        }
    }



    if (destined->left != NULL)(destined->left)->right = destined->right;
    if (destined->right != NULL)(destined->right)->left = destined->left;

    destined->left = NULL;
    destined->right = NULL;
    destined->par = NULL;


    return true;
}

bool consolidate_fib_link(struct fib_node * node_root)
{
    if (fib_arr[node_root->degree] == NULL) {
        fib_arr[node_root->degree] = node_root;
        return false;
    }
    else
    {
        struct fib_node * temp = fib_arr[node_root->degree];
        fib_arr[node_root->degree] = NULL;


        if (node_root->dist < temp->dist || node_root == fib_root)
        {
            fib_cut_remove(temp);
            fib_add_child(node_root, temp);
            if (fib_arr[node_root->degree] != NULL)consolidate_fib_link(node_root);
            else
                fib_arr[node_root->degree] = node_root;
        }
        else
        {
            fib_cut_remove(node_root);
            fib_add_child(temp, node_root);

            if (fib_arr[temp->degree] != NULL)consolidate_fib_link(node_root);
            else
                fib_arr[temp->degree] = temp;
        }
        return true;
    }
}


void fib_decreaseKey(int new_dist, struct fib_node * fib_vertex)
{
    fib_vertex->dist = new_dist;

    struct fib_node * temp = fib_vertex;
    if (fib_vertex->par != NULL)
    {
        fib_cut_remove(fib_vertex);
        fib_add_sibling(fib_root, fib_vertex);
    }
    fib_vertex = temp;

    if (fib_vertex->dist < fib_root->dist)fib_root = fib_vertex;
}


struct fib_node* fib_deleteMin()
{
    struct fib_node *temp;
    temp = fib_left_sibling(fib_root->child);

    struct fib_node * temp2 = NULL;


    while (temp != NULL)
    {
        temp2 = temp->right;
        fib_cut_remove(temp);
        fib_add_sibling(fib_root, temp);
        temp = temp2;
    }

    temp = fib_left_sibling(fib_root);

    if (temp == fib_root)
    {
        if (fib_root->right != NULL) {
            temp = fib_root->right;
        }
        else
        {
            struct fib_node * extra = fib_root;
            fib_cut_remove(fib_root);
            fib_root = NULL;
            return extra;
        }
    }

    struct fib_node * extra = fib_root;

    fib_cut_remove(fib_root);
    fib_root = temp;



    for (int i = 0; i < 100; i++)
    {
        fib_arr[i] = NULL;
    }


    while (temp != NULL)
    {
        if (temp->dist < fib_root->dist)
        {
            fib_root = temp;
        }

        temp2 = temp->right;
        consolidate_fib_link(temp);
        temp = temp2;
    }

    return extra;
}



/////////////////////////////////////////////////////fibonacci heap ends here ///////////////////////////

int adj[minN][minN];
int h[minN];

void fib_dijkstra(int n, int source)
{
    fib_root = NULL;
    struct fib_node * arr_ptr[n + 5];


    for (int i = 1; i <= n; i++)
    {
        arr_ptr[i] = fib_push(inf, i);
        if (i == source)arr_ptr[i]->dist = 0;
        else
        {
            arr_ptr[i]->dist = inf;
        }

        arr_ptr[i]->id = i;
    }

    int dist[n + 5];
    int vis[n + 5];
    for (int i = 1; i <= n; i++)
    {
        dist[i] = inf;
        vis[i] = 0;
    }

    dist[source] = 0;


    while (!fib_isEmpty())
    {
        struct fib_node * min_node = fib_deleteMin(fib_root);
        int u = min_node->id;

        if (vis[u])continue;
        vis[u] = 1;

        for (int v = 1; v <= n; v++)
        {
            if (adj[u][v] == inf)continue;

            int w = adj[u][v];

            if (dist[v] > dist[u] + w)
            {
                dist[v] = dist[u] + w;

                fib_decreaseKey(dist[v], arr_ptr[v]);
            }
        }
    }



    for (int i = 1; i <= n; i++)
    {
        if (dist[i] == inf)
            printf("Vertex %d is NOT reachable from source %d.\n", i, source);
        else
            printf("Cost of reaching vertex %d from source %d is %d.\n", i, source, dist[i] + h[i] - h[source]);

    }
}

int neg = 0;

void bellman_ford(int n, int source)
{

    for (int i = 1; i <= n; i++)
    {
        h[i] = inf;
    }
    h[source] = 0;


    for (int j = 0; j <= n; j++)
    {
        int extra = 0;

        for (int u = 0; u <= n; u++)
        {
            for (int v = 1; v <= n; v++)
            {
                int w = adj[u][v];

                if (w == inf || u == v)continue;
                if (h[u] != inf && h[u] + w < h[v])
                {
                    h[v] = h[u] + w;
                    extra++;
                }
            }

            if (extra == 0)break;
        }


        for (int u = 0; u <= n; u++)
        {
            for (int v = 1; v <= n; v++)
            {
                int w = adj[u][v];

                if (h[u] != inf && h[u] + w < h[v])
                {
                    neg = 1;
                    break;
                }
            }
        }
    }
}



void johnson_algorithm(int n)
{
    int src = 0;

    for (int i = 1; i <= n; i++)
    {
        adj[0][i] = 0;
    }


    bellman_ford(n, 0);


    if (neg)
    {
        printf("Graphg contains negative cycle\n");
        return;
    }


    for (int u = 1; u <= n; u++)
    {
        for (int v = 1; v <= n; v++)
        {
            int w = adj[u][v];
            if (w == 0 || w == inf)continue;
            adj[u][v] = w + h[u] - h[v];

        }
    }




    printf("Enter the following choices of heaps for Running Dijkstra Algorithm.\n");

    printf("1. Binomial Heap.\n2.Binary Heap.\n3.Fibonacci Heap.\n");
    int choice; scanf("%d", &choice);


    if (choice == 1)
    {

    }
    else if (choice == 2)
    {

    }
    else if (choice == 3)
    {
        for (int i = 1; i <= n; i++)fib_dijkstra(n, i);
    }
    else {
        printf("Invalid Choice\n");
    }
}



int main(int argc, char* argv[])
{

    printf("Enter number of nodes in a graph\n");
    int n; scanf("%d", &n);


    for (int i = 1; i <= n; i++)
    {
        for (int j = 1; j <= n; j++) {
            adj[i][j] = inf;
            if (i == j)adj[i][j] = 0;
        }
    }

    printf("Enter the type of graph. Tpye U if Graph is Undirected OR Type D if Graph is Undirected.\n");
    char type;
    scanf(" %c", &type);

    if (type == 'U')
    {
        printf("Enter edges one by one of your Undirected graph. Once done, press Q:\n");

        while (1)
        {
            char ch;
            scanf(" %c", &ch);
            if (ch == 'Q')
            {
                break;
            }

            int u, v;
            char temp;
            int w;
            scanf("%d%c%d%c%d%c", &u, &temp, &v, &temp, &w, &temp);
            if (u > n || v > n || u <= 0 || v <= 0)
            {
                printf("Please enter Valid node\n");
                continue;
            }

            // add edges
            adj[u][v] = w;
            adj[v][u] = w;


        } johnson_algorithm(n);
    }
    else if (type == 'D')
    {
        printf("Enter edges one by one of your Undirected graph. Once done, press Q:\n");

        while (1)
        {
            char ch;
            scanf(" %c", &ch);
            if (ch == 'Q')
            {
                break;
            }

            int u, v;
            char temp;
            int w;
            scanf("%d%c%d%c%d%c", &u, &temp, &v, &temp, &w, &temp);
            if (u > n || v > n || u <= 0 || v <= 0)
            {
                printf("Please enter Valid node\n");
                continue;
            }

            // add edges
            adj[u][v] = w;


        } johnson_algorithm(n);
    }
    else
    {
        printf("Invalid Choice\n");
    }
}


