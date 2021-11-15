#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>
#include<malloc.h>
#define inf 2147483647

struct vertex{
    int vertex_num;
    int distance;
};




void shortestPath_binary_heap(int n,int adj[][n+1],int reweighted_edges[][n+1],int dist,int src)
{
    while(!pq.empty())
    {
        int u=pq.top().distance;
        pq.pop();


        for(int i=1;i<=n;i++)
        {
            if(reweighted_edges[src][i]==0)continue;

            int v=i;
            
            int weight=reweighted_edges[src][i];

            if(dist[v]>dist[u]+weight)
            {
                dist[v]=dist[u]+weight;
                struct vetex temp;
                temp.disntace=dist[v];
                temp.vertex_num=v;
                pq.push(temp);
            }
        }
    }
}

void shortestPath_binomial_heap(int n,int adj[][n+1],int reweighted_edges[][n+1],int dist,int src)
{
    while(!pq.empty())
    {
        int u=pq.top().distance;
        pq.pop();


        for(int i=1;i<=n;i++)
        {
            if(reweighted_edges[src][i]==0)continue;

            int v=i;
            
            int weight=reweighted_edges[src][i];

            if(dist[v]>dist[u]+weight)
            {
                dist[v]=dist[u]+weight;
                struct vetex temp;
                temp.disntace=dist[v];
                temp.vertex_num=v;
                pq.push(temp);
            }
        }
    }
}


void shortestPath_fibonacci_heap(int n,int adj[][n+1],int reweighted_edges[][n+1],int dist,int src)
{
    while(!pq.empty())
    {
        int u=pq.top().distance;
        pq.pop();


        for(int i=1;i<=n;i++)
        {
            if(reweighted_edges[src][i]==0)continue;

            int v=i;
            
            int weight=reweighted_edges[src][i];

            if(dist[v]>dist[u]+weight)
            {
                dist[v]=dist[u]+weight;
                struct vetex temp;
                temp.disntace=dist[v];
                temp.vertex_num=v;
                pq.push(temp);
            }
        }
    }
}

struct bellman_edges{
    int src;
    int dest;
    int weight;
};
//Bellman Ford Algorithm

//Dijkstra Algorithm

void Dijkstra(int n,int adj[][n+1],int reweighted_edges[][n+1],int src,int choice)
{
    int dist[n+1];

    for(int i=1;i<=n;i++)dist[i]=inf;

    dist[src]=0;

    if(choice==1)
    {
        shortestPath_binary_heap(n,adj,reweighted_edges,dist,src);
    }
    else if(choice==2)
    {
        shortestPath_binomial_heap(n,adj,reweighted_edges,dist,src);
    }
    else 
    shortestPath_fibonacci_heap(n,adj,reweighted_edges,dist,src);

    for(int i=1;i<=n;i++)
    {
        if(dist[i]==inf)
        printf("It is not reachable from source");
        else 
        printf("Distance from %d to %d is found to be:",src,dist[i]);
    }


}

void takeEdges(int n,int adj[][n+1])
{
    printf("Enter the number of edges.\n");
    int m;
    scanf("%d",&m);
    printf("Enter edges along with respective weights with node values from 1 to n only.\n");
    for(int i=0;i<m;i++)
    {
        int u,v,weight;
        scanf("%d%d%d",u,v,weight);
        adj[u][v]=weight;
    }
}

void Bellman_Ford(int n,int adj[][n+1],int reweighted_edges[][n+1])
{
    int dummy_graph[n+1][n+1];

    for(int i=0;i<=n;i++)
    {
        for(int j=0;j<=n;j++)dummy_graph[i][j]=adj[i][j];
    }

    int h[n+1];

    for(int i=1;i<=n;i++){
        h[i]=inf;
    }
    h[0]=0;

    struct bellman_edges* edges;


    int m=0;
    for(int i=0;i<=n;i++)
    {
        for(int j=1;j<=n;j++)
        {
            if(adj[i][j]==inf)continue;
            
            struct bellman_edges temp;
            edges=(struct bellman_edges*)malloc((m+1)*sizeof(struct bellman_edges));
            temp.src=i;
            temp.dest=j;
            temp.weight=adj[i][j];
            edges[m]=temp;
            m++;
        }
    }

    for(int i=1;i<=n;i++)
    {
        for(int j=0;j<m;j++)
        {
            int source=edges[j].src;
            int dest=edges[j].dest;
            int wgt=edges[j].weight;    
            if(h[source]!=inf && (h[source]+wgt<h[dest]))h[dest]=h[source]+wgt;
        }
    }


    for(int i=1;i<=n;i++)
    {
        for(int j=1;j<=n;j++)
        {
            reweighted_edges[i][j]=0;
        }
    }

    for(int i=1;i<=n;i++)
    {
        for(int j=1;j<=n;j++)
        {
            if(reweighted_edges[i][j]==0)continue;
            reweighted_edges[i][j]=adj[i][j]+h[i]-h[j];
        }
    }
}


int main()
{
    printf("Input the number of vertices in graph:\n ");

    int n;
    scanf("%d",&n);

    int adj[n+1][n+1];

    for(int i=0;i<=n;i++)
    {
        for(int j=0;j<=n;j++)
        {
            if(i==j)adj[i][j]=0;
            else 
            adj[i][j]=inf;
        }
    }

    takeEdges(n,adj);

    int reweighted_edges[n+1][n+1];
    
    Bellman_Ford(n,adj,reweighted_edges);


    printf("Enter the following choices of heaps for Running Dijkstra Algorithm.\n");

    printf("1. Binomial Heap.\n2.Binary Heap.\n3.Fibonacci Heap.\n");
    int choice;scanf("%d",choice);

    for(int i=1;i<=n;i++)
    Dijkstra(n,adj,reweighted_edges,i,choice);
}