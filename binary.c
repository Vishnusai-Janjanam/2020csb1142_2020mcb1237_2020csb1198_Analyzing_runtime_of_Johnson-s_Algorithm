#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>
#include<malloc.h>
#include<math.h>



struct pair {
    int F;
    int S;
};

struct pair* bin_heap;
int bin_N;

void bin_heapify(int bin_x)
{
    int bin_size= bin_N;
    int bin_small = bin_x;
    int bin_l = 2*bin_x +1, bin_r = 2*bin_x+2;

    if(bin_l< bin_size && bin_heap[bin_l].F < bin_heap[bin_small].F)
        bin_small = bin_l;
    if(bin_r< bin_size && bin_heap[bin_r].F < bin_heap[bin_small].F)
        bin_small = bin_r;

    if(bin_x != bin_small)
    {
        struct pair temp;
        temp = bin_heap[bin_x];
        bin_heap[bin_x]= bin_heap[bin_small];
        bin_heap[bin_small]= temp;
        bin_heapify(bin_small);
    }

}

void bin_insert(int bin_val, int bin_dat)
{

    if(bin_N==0)
    {
        bin_heap[bin_N].F = bin_val;
        bin_heap[bin_N].S = bin_dat;
        bin_N++;
    }
    else
    {
        bin_heap[bin_N].F = bin_val;
        bin_heap[bin_N].S = bin_dat;
        bin_N++;
        for(int i = bin_N/2-1; i>=0; i--)
        {
            bin_heapify(i);
        }
    }
}

void bin_pop()
{
    int bin_size = bin_N;
    if(bin_N==0)
        return;
    if(bin_N==1)
    {
        bin_N--;
    }
    else
    {
        struct pair temp = bin_heap[bin_N-1];
        bin_heap[bin_N-1] = bin_heap[0];
        bin_heap[0] = temp;
        bin_N--;

        for(int i=bin_N/2-1; i>=0; i--)
            bin_heapify(i);
    }
}

void bin_decreaseKey(int bin_val, int bin_dat)
{
    bin_heap[bin_N].F = bin_val;
    bin_heap[bin_N].S = bin_dat;
    bin_N++;

    if(bin_N>1)
    {
        for(int i= bin_N/2-1; i>=0; i--)
            bin_heapify(i);
    }
}

void bin_dijkstra(int bin_src, int n)
{
    bin_insert(0, src);

    int dist[n + 5];
    int vis[n + 5];

    for (int i = 1; i <= n; i++)
    {
        dist[i] = inf;
        vis[i] = 0;
    }

    dist[bin_src] = 0;

    struct pair cur;

    while(bin_N>0)
    {
        cur = bin_heap[0];
        bin_pop();

        struct adj_list *temp;
        temp = arr[cur.S];

        while(temp!=NULL)
        {
            int w = temp->weight;
            int v = temp->node;
            if (dist[v] > dist[u] + w)
            {
                dist[v] = dist[u] + w;

                bin_decreaseKey(dist[v], v);
            }

            temp = temp->next;
        }


    }
}


void main()
{
    bin_heap = malloc(1000*sizeof(struct pair));
    bin_N = 0;
    for(int i= 100; i>0; i--)
    {
        bin_insert(i, 59);
    }

    while(bin_N)
    {
        printf("%d\n", bin_heap[0].F);
        bin_pop();
    }



}
