#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <malloc.h>
#include <math.h>
#include <time.h>
#include<sys/time.h>

///////////////////////////////////////////////////////////////////////////////////////////////////
#define inf 10000000
#define N 100000
#define minN 5050



struct adj_list //creation of Adjacency List Node
{
    int node;
    struct adj_list *next;
    int weight;
};

struct adj_list *arr[N]; //initializing size of adjacency list
void addEdges(int u, int v, int weight) //adding edges
{
    struct adj_list *temp;
    temp = (struct adj_list *)malloc(sizeof(struct adj_list)); //creating new node for list
    temp->node = v;
    temp->next = NULL;
    temp->weight = weight;
    if (arr[u] == NULL) //if List was empty
    {
        arr[u] = temp;
    }
    else //Inserting Node into head of list for Simplicity
    {
        temp->next = arr[u];
        arr[u] = temp;
    }
}


int h[minN];//required for Bellman Ford Algorithm to reweight edges


/////////////////////////////Heaps ////////////////////////////////////////////////////////////

//////////BINARY HEAP////////////////////////////////////////

struct pair {
    int F;
    int S;
};

struct pair* bin_heap;
int bin_N;
int map[minN];
void bin_heapify(int bin_x)
{
    int bin_size = bin_N;
    int bin_small = bin_x;
    int bin_l = 2 * bin_x + 1, bin_r = 2 * bin_x + 2;

    if (bin_l < bin_size && bin_heap[bin_l].F < bin_heap[bin_small].F)          //comparison
        bin_small = bin_l;
    if (bin_r < bin_size && bin_heap[bin_r].F < bin_heap[bin_small].F)
        bin_small = bin_r;

    if (bin_x != bin_small)                                                     //if change needed
    {
        struct pair temp;
        temp = bin_heap[bin_x];
        bin_heap[bin_x] = bin_heap[bin_small];                                  //value swap
        map[bin_heap[bin_x].S] = bin_x;
        bin_heap[bin_small] = temp;
        map[bin_heap[bin_small].S] = bin_small;
        bin_heapify(bin_small);                                                 //recursive call
    }

}

void bin_insert(int bin_val, int bin_dat)
{

    if (bin_N == 0)             // initial heap
    {
        bin_heap[bin_N].F = bin_val;
        bin_heap[bin_N].S = bin_dat;
        map[bin_heap[bin_N].S] = 0;
        bin_N++;
    }
    else
    {
        bin_heap[bin_N].F = bin_val;
        bin_heap[bin_N].S = bin_dat;
        map[bin_heap[bin_N].S] = bin_N;
        bin_N++;
        for (int i = bin_N / 2 - 1; i >= 0; i--)
        {
            bin_heapify(i);
        }
    }
}

void bin_pop()
{
    int bin_size = bin_N;
    if (bin_N == 0)
        return;
    if (bin_N == 1)
    {
        map[bin_heap[0].S] = -1;
        bin_N--; //size set to 0
    }
    else
    {
        struct pair temp = bin_heap[bin_N - 1];
        bin_heap[bin_N - 1] = bin_heap[0];  // min element removed
        bin_heap[0] = temp;
        map[bin_heap[bin_N - 1].S] = -1;
        bin_N--;    // size reduced by 1

        for (int i = bin_N / 2 - 1; i >= 0; i--)
            bin_heapify(i);
    }
}

void bin_decreaseKey(int bin_val, int bin_dat)
{
    if (map[bin_dat] < 0)
    {
        bin_heap[bin_N].F = bin_val;
        bin_heap[bin_N].S = bin_dat;
        bin_N++;
        for (int i = bin_N / 2 - 1; i >= 0; i--)
            bin_heapify(i);
    }
    else
    {
        int i = map[bin_dat];       // current index of key in heap
        if (bin_val > bin_heap[i].F)
            return;
        bin_heap[i].F = bin_val;    //new value set
        while (i != 0)
        {
            if (bin_heap[i].F < bin_heap[i / 2].F)  //new value gets abosorbed into the heap
            {
                struct pair temp = bin_heap[i];
                bin_heap[i] = bin_heap[i / 2];
                bin_heap[i / 2] = temp;
                map[bin_heap[i].S] = i;
                map[bin_heap[i / 2].S] = i / 2;
            }
            else
                break;
        }
    }
}

void bin_dijkstra(int bin_src, int n)
{
    bin_insert(0, bin_src);

    int dist[n + 5];
    int vis[n + 5];

    for (int i = 1; i <= n; i++)
    {
        dist[i] = inf;
        vis[i] = 0;
    }

    dist[bin_src] = 0;

    struct pair cur;

    while (bin_N > 0) //while heap has elements
    {
        cur = bin_heap[0];
        bin_pop();
        int u = cur.S;
        struct adj_list *temp;
        temp = arr[cur.S];      //adjacency list of current node

        while (temp != NULL)
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


/////////////////////////BINary heap////////////////////////////////


////////////////////////BINOMIAL HEAP//////////////////////////////////////


struct node {                                   //This stores the pair vertex and weight
    int vertex, weight;
}
typedef node;
//const int inf = 107374;
struct binomialNode {       // This is the node of the binomial heap
    node key;
    int degree;
    struct binomialNode* parent , *fc, *lc, *ls, *rs;
}
// fc means the first child, lc means the last child, ls means the left sibling and rs means the right sibling
typedef binomialNode;
binomialNode *binomialHead = NULL;
binomialNode** map1;
binomialNode* new_binomialNode(void) {          // This function creates a new binomial node and initiaizes the degree to 0 and all pointers to NULL.
    binomialNode* h1  = (binomialNode*)malloc(sizeof(binomialNode));
    h1->degree = 0, h1->parent = h1->lc = h1->rs = h1->ls = h1->fc = NULL;
    return h1;
}

binomialNode* get_min_binomial_heap(void) {     //this function finds the minimum element in the heap
    binomialNode* curr =  binomialHead, *result;
    result = curr;
    if (result == NULL) return result;
    node min = curr->key;
    while (curr) {                                      //Searching all the roots(as the roots contain the minimum element of the respective tree)
        if (curr->key.weight < min.weight) {
            min = curr->key;
            result = curr;
        }
        curr = curr->rs;
    }
    return result;
}
binomialNode* union_binomial_heap(binomialNode* h1, binomialNode *h2) {         // This function unites two binomial heaps
    binomialNode* newHeap = NULL, *curr1 = h1, *curr2 = h2;
    binomialNode* curr3 = newHeap;
    while (curr1 && curr2) {
        //Adding tree in increasing order of degree
        if (curr1->degree <= curr2->degree) {
            if (curr3 ==    NULL) {
                curr3 = newHeap = curr1;
            }
            else {
                curr3->rs = curr1;
                curr3 = curr3->rs;
            }
            curr1 = curr1->rs;
        }
        else {
            if (curr3 ==    NULL) {
                curr3 = newHeap = curr2;
            }
            else {
                curr3->rs = curr2;
                curr3 = curr3->rs;
            }
            curr2 = curr2->rs;
        }

    }
    // Adding the leftover trees in the first binomial heap.
    while (curr1) {
        if (curr3 == NULL)curr3 = newHeap = curr1;
        else {
            curr3->rs = curr1;
            curr3 = curr3->rs;
        }
        curr1 = curr1->rs;
    }
    // Adding the leftover trees in the first binomial heap.

    while (curr2) {
        if (curr3 == NULL)curr3 = newHeap = curr2;
        else {
            curr3->rs = curr2;
            curr3 = curr3->rs;
        }
        curr2 = curr2->rs;
    }
    return newHeap;
}
binomialNode* merge_binomial_tree(binomialNode*t1, binomialNode* t2) {      //This function merges two binomial nodes with same degree
    if (t1->key.weight < t2->key.weight) { // When prev key is smaller then the ptr key.
        binomialNode* temp = t1->lc;    //Storing the last child of the prev node.
        t1->lc = t2;        //Updating the last child of prev node
        (t1->degree)++;     //Updating the degree of ptr node.
        t2->parent = t1;    //Adding parent of ptr node
        t2->rs = temp;      //making temp the right sibling of ptr node.
        if (temp) temp->ls = t2;
        if (t1->degree == 1) t1->fc = t2;  //If t2 is the first child.
        return t1;
    }
// this executes when key of ptr is samller than key of prev
    binomialNode* temp = t2->lc;
    (t2->degree)++;
    t1->parent = t2;
    t2->lc = t1;
    t1->rs = temp;
    if (temp) temp->ls = t1;
    if (t2->degree == 1) t2->fc = t1;
    return t2;
}
void merge_binomial_heap(void) {        //This function will merge all the heaps with same degree which are present initially and during the process too.
    if (binomialHead == NULL || binomialHead->rs == NULL)return;
    //nextn is next of next
    binomialNode *prev = NULL, *ptr = binomialHead, *next = ptr->rs, *nextn = next->rs;
    while (next) {
        if (next->degree != ptr->degree) {// If degree is not same then update the pointes.
            prev = ptr;
            ptr = next;
            next = nextn;
            if (nextn)nextn = nextn->rs;
        }
        else if (nextn && nextn->degree == next->degree) { // If nextn degree is equal to ptr then updating hte pointers.
            prev = ptr;
            ptr = next;
            next = nextn;
            nextn = nextn->rs;
        }
        else {                                                      //degree of ptr == degree of next.
            if (prev == NULL) {                             //When first two node are to be merged
                binomialNode* t1 = nextn;
                ptr = merge_binomial_tree(ptr, next);
                binomialHead  = ptr;
                ptr->rs = t1;
                next = t1;
                if (next) nextn = next->rs;
            }
            else {                                              //if merging nodes are somewhere in between
                if (ptr->key.weight < next->key.weight) {
                    binomialNode* t1 = next->rs;
                    ptr = merge_binomial_tree(ptr, next);
                    ptr->rs = t1;
                    next = t1;
                }
                else {
                    ptr = merge_binomial_tree(ptr, next);
                    prev->rs = ptr;
                    next = ptr->rs;
                    if (next) nextn = next->rs;
                }
            }
        }
    }
}
void insert_binomial_node(node x) {     //Function to insert a binomial node.
    binomialNode* h1 = new_binomialNode();
    h1->key = x;
    map1[x.vertex] = h1;
    binomialHead = union_binomial_heap(h1, binomialHead);   //Taking union of the two heaps
    merge_binomial_heap();              //Performing the merge operation
}
void delete_min_binomial(void) {                //Function to delete the min node of the binomial heap
    binomialNode* min_node = get_min_binomial_heap();
    binomialNode* curr = binomialHead, *prev = NULL, *temp_h1;
    while (curr) {  //Finding the prev node of min node.
        if (curr == min_node) {
            if (prev == NULL) {
                binomialHead = curr->rs;
            }
            else {
                prev->rs = curr->rs;
            }
            break;
        }
        prev = curr;
        curr = curr->rs;
    }
    temp_h1 = curr->fc;
    curr = temp_h1;
    while (curr) {
        curr->parent = NULL;
        curr->rs = curr->ls;
        curr->ls = NULL;
        curr = curr->rs;
    }
    //Updating the heap
    binomialHead = union_binomial_heap(binomialHead, temp_h1);
    merge_binomial_heap();
}
void decrease_key_binomial_heap(binomialNode* pos, int new_key) {//Function to decrease key
    pos->key.weight = new_key;
    binomialNode* par = pos->parent;
    while (par && par->key.weight > pos->key.weight) {
        //swapping data and changing the map of the node for the vertices
        int par_vertex = par->key.vertex;
        int par_weight = par->key.weight;

        par ->key.vertex = pos->key.vertex;
        par->key.weight = pos->key.weight;

        map1[pos->key.vertex] = par;

        pos->key.vertex = par_vertex;
        pos->key.weight = par_weight;
        map1[par_vertex] = pos;
        //going up the tree in such a way that binary heap property is maintained.
        pos = par;
        par = pos->parent;
    }
}
void dijkstra_binomial(int s, int n) {//Dijsktra using binomial heap
    node temp;
    map1 = (binomialNode**) malloc(sizeof(binomialNode*) * (n + 1));//Stores the position of each vertex in the binomial heap
    int vst[n + 1], dis[n + 1];     //visit and distance array
    temp.vertex = s, temp.weight = 0;
    insert_binomial_node(temp);//inserting the  source node.
    for (int i = 1; i <= n; i++) {
        map1[i] = NULL;
        vst[i] = 0;
        dis[i] = inf;
        if (i != s) {
            temp.vertex = i;
            temp.weight = inf;
            insert_binomial_node(temp);     // Adding all other nodes with best distance as inf
        }
    }
    dis[s] = 0;
    while (binomialHead) {
        binomialNode* min_node = get_min_binomial_heap();//Finding the min node
        delete_min_binomial();      //Deleting the min node
        int u = min_node->key.vertex;
        if (vst[u]) continue;
        vst[u] = 1;
        struct adj_list* curr = arr[u];
        while (curr) {      //traversing the adjacency list of the the vertex u.
            if (dis[curr->node] > dis[u] + curr->weight) {
                dis[curr->node] = dis[u] = curr->weight;
                decrease_key_binomial_heap(map1[curr->node], dis[curr->node]); //decreasing the key.
            }
            curr = curr->next;
        }
    }
    //Printing the results of the dijkstra.
    for (int i = 1; i <= n; i++)
    {
        if (dis[i] == inf)
            printf("Vertex %d is NOT reachable from source %d.\n", i, s);
        else
            printf("Cost of reaching vertex %d from source %d is %d.\n", i, s, dis[i] + h[i] - h[s]);
    }
}

/////////////////Fibonacci Heap/////////////////////////////////
struct fib_node//creation of fibonacci heap node
{
    int dist;//weight of fiboacci node
    int vertex;//node number
    int degree;//degree of node in fibonacci tree
    int id;//node id at time of creation
    struct fib_node *par;//parent of node in fibonacci tree
    struct fib_node *child;//child of node in fibonacci tree
    struct fib_node *left;////leftnode  of node in fibonacci tree
    struct fib_node *right;//right node of node in fibonacci tree
};


struct fib_node *fib_arr[minN];//declaration of fibonacci array storing pointers to nodes
struct fib_node *fib_root;//root to the fibonacci tree


struct fib_node *init_fib_node(int distance, int vertex)//function that returns a new node created for given arguements
{
    struct fib_node *temp;
    temp = (struct fib_node *)malloc(sizeof(struct fib_node));//memory allocation
    temp->child = NULL;
    temp->par = NULL;
    temp->left = NULL;
    temp->right = NULL;//assigning NULL to pointers
    temp->degree = 0;
    temp->vertex = vertex;//assigning the value of node
    temp->dist = distance;// assigning distance value to fibonacci nodes
    return temp;
}

struct fib_node *fib_left_sibling(struct fib_node *temp)//function for finding leftmost sibling of node passed in arguement
{
    if (temp == NULL)//if node is NULL
        return temp;
    while (temp->left != NULL)//iterating the list of same degree nodes in fibonacci tree to find leftmost sibling
    {
        temp = temp->left;
    }
    return temp;//returning leftmost node
}

struct fib_node *fib_right_sibling(struct fib_node *temp)//function for finding rightmost sibling of node passed in arguement
{
    if (temp == NULL)
        return temp;
    while (temp->right != NULL)//iterating the list of same degree nodes in fibonacci tree to find rightmost sibling
    {
        temp = temp->right;
    }
    return temp;
}

void  fib_add_sibling(struct fib_node *destined, struct fib_node *new_node)//function to add sibling to same degree node
{
    struct fib_node *temp;
    temp = fib_right_sibling(destined);//destined is node where node is to be added

    if (temp == NULL)
        return;

    temp->right = new_node;
    new_node->left = temp;
    new_node->par = destined->par;//adding new node wuth same degree
    new_node->right = NULL;

    if (destined->par != NULL)
        (destined->par)->degree = (destined->par)->degree + 1;//if parent did not had child earlier, increasing it's degree
}

void fib_add_child(struct fib_node *destined, struct fib_node *new_node)//function to add new node to a given node
{
    if (destined == NULL)
        return ;

    if (destined->child != NULL)
    {
        fib_add_sibling(destined->child, new_node);//if given node has any child then we add new node to same branch as of child of given node
    }
    else//else adding this as its child
    {
        destined->child = new_node;
        destined->degree = 1;
        new_node->par = destined;
    }

}

bool fib_isEmpty()//function to check if fibonacci heap is empty or not
{
    if (fib_root == NULL)
        return true;
    else
        return false;
}

struct fib_node *fib_push(int distance, int vertex)// function to add new node in the heap
{
    struct fib_node *temp;
    temp = init_fib_node(distance, vertex);//getting new node pointer after assiging parameters

    if (fib_root == NULL)
    {
        fib_root = temp;
        return temp;
    }

    fib_add_sibling(fib_root, temp);//if root is not empty then adding new node as its sibling

    if (fib_root->dist > temp->dist)//if new node has smaller dist. parameter then making it as root
        fib_root = temp;

    return temp;
}

void fib_cut_remove(struct fib_node *destined)//function to cut the node from branch when required
{
    if (destined->par != NULL)
    {
        ((destined->par)->degree) = ((destined->par)->degree) - 1;//decreasing degree of parent of given node to be removed
        if (destined->left != NULL)//removing the node and its link from rest of branch
        {
            ((destined)->par)->child = destined->left;
        }
        else if (destined->right != NULL)
        {
            ((destined)->par)->child = destined->right;
        }
        else
        {
            destined->par->child = NULL;//if it was single node
        }
    }

    if (destined->left != NULL)//adjusting the remaining branch afer removing the node
        (destined->left)->right = destined->right;
    if (destined->right != NULL)
        (destined->right)->left = destined->left;

    destined->left = NULL;
    destined->right = NULL;
    destined->par = NULL;
}

void consolidate_fib_link(struct fib_node *node_root)//adjusting nodes of fibonacci heap while decreasing its key
{
    if (fib_arr[node_root->degree] == NULL)// if node is not yet marked
    {
        fib_arr[node_root->degree] = node_root;
    }
    else
    {
        struct fib_node *temp = fib_arr[node_root->degree];
        fib_arr[node_root->degree] = NULL;

        if (node_root->dist < temp->dist || node_root == fib_root)//if node has smaller key than root key
        {
            fib_cut_remove(temp);//cutting the node from current position
            fib_add_child(node_root, temp);//adding it to the top level branch of root
            if (fib_arr[node_root->degree] != NULL)
                consolidate_fib_link(node_root);//recursively calling function untill node is unmarked
            else
                fib_arr[node_root->degree] = node_root;//marking the last remaining node while top up recursion
        }
        else
        {
            fib_cut_remove(node_root);
            fib_add_child(temp, node_root);//node is added to given node from array of pointers of fibonacci heap

            if (fib_arr[temp->degree] != NULL)
                consolidate_fib_link(node_root);//adjusting the node by recursion
            else
                fib_arr[temp->degree] = temp;
        }
    }
}

void fib_decreaseKey(int new_dist, struct fib_node *fib_vertex)//decrease key function
{
    fib_vertex->dist = new_dist;//assigning the changed dist parameter to node

    struct fib_node *temp = fib_vertex;
    if (fib_vertex->par != NULL)//cutting the current node from current position and adding it to top branch
    {
        fib_cut_remove(fib_vertex);
        fib_add_sibling(fib_root, fib_vertex);
    }
    fib_vertex = temp;

    if (fib_vertex->dist < fib_root->dist)//assigning root new value if it has smaller parameter dist
        fib_root = fib_vertex;
}

struct fib_node *fib_deleteMin()///function that deletes the min key node and returning its pointer for information
{
    struct fib_node *temp;
    temp = fib_left_sibling(fib_root->child);//for sake of finding new min key

    struct fib_node *temp2 = NULL;

    while (temp != NULL)
    {
        temp2 = temp->right;
        fib_cut_remove(temp);
        fib_add_sibling(fib_root, temp);
        temp = temp2;
    }

    temp = fib_left_sibling(fib_root);//finding leftmost siblilng of node

    if (temp == fib_root)
    {
        if (fib_root->right != NULL)
        {
            temp = fib_root->right;
        }
        else
        {
            struct fib_node *extra = fib_root;
            fib_cut_remove(fib_root);
            fib_root = NULL;
            return extra;
        }
    }

    struct fib_node *extra = fib_root;

    fib_cut_remove(fib_root);//cutting the min root and assigning new min key as root
    fib_root = temp;

    for (int i = 0; i < 100; i++)// max height of fibonacci tree is logn hence assigning NULL to all values upto 100
    {
        fib_arr[i] = NULL;
    }

    while (temp != NULL)//iterating the branch to find minimum key
    {
        if (temp->dist < fib_root->dist)
        {
            fib_root = temp;
        }

        temp2 = temp->right;
        consolidate_fib_link(temp);// consolidating the current node
        temp = temp2;
    }

    return extra;
}

/////////////////////////////////////////////////////fibonacci heap ends here ///////////////////////////


void fib_dijkstra(int n, int source)//dijkstra algorithm that uses Fibonacci Heap
{
    fib_root = NULL;//declaring NULL pointer to fibroot
    struct fib_node *arr_ptr[n + 5];//an array of pointer to fibonacci nodes to further pass as an arguement for various functions

    for (int i = 1; i <= n; i++)
    {
        arr_ptr[i] = fib_push(inf, i);//initially pushing all nodes with value equal to infinity
        if (i == source)//if source then assigning it's parameter as 0
            arr_ptr[i]->dist = 0;
        else
        {
            arr_ptr[i]->dist = inf;//else infinity
        }

        arr_ptr[i]->id = i;//assigning node id to be same as vertex
    }

    int dist[n + 5];//declaring the array of dist parameter for every node
    int vis[n + 5];//declaring visit array
    for (int i = 1; i <= n; i++)//assigning dist and vis values
    {
        dist[i] = inf;
        vis[i] = 0;
    }

    dist[source] = 0;//assigning the source as 0

    while (!fib_isEmpty())//iterating the fib heap untill it becomes empty
    {
        struct fib_node *min_node = fib_deleteMin(fib_root);//getting the minimum key
        int u = min_node->id;//getting the id of vertex added

        if (vis[u])//if node is already visited then it skips
            continue;
        vis[u] = 1;//marking it as visited

        struct adj_list *temp;
        temp = arr[u];//getting pointer of current node

        while (temp != NULL)//iterating the edge list of node u
        {
            int w = temp->weight;
            int v = temp->node;
            if (dist[v] > dist[u] + w)//checking required condition to swap
            {
                dist[v] = dist[u] + w;

                fib_decreaseKey(dist[v], arr_ptr[v]);//decreasing key of node
            }

            temp = temp->next;
        }
    }

    for (int i = 1; i <= n; i++) //printing distance of each node from source (shotest path)
    {
        if (dist[i] == inf)
            printf("Vertex %d is NOT reachable from source %d.\n", i, source);
        else
            printf("Cost of reaching vertex %d from source %d is %d.\n", i, source, dist[i] + h[i] - h[source]);

    }
}

int neg = 0;//for checking negative cycle in graph

void bellman_ford(int n, int source)
{

    for (int i = 1; i <= n; i++)//initializing h array as inf
    {
        h[i] = inf;
    }
    h[source] = 0;//assinging 0 as dummy node

    for (int j = 0; j <= n; j++)
    {
        int extra = 0;//checking if h is assigned a value or not

        for (int u = 0; u <= n; u++)
        {

            struct adj_list *temp;
            temp = arr[u];

            while (temp != NULL)
            {
                int w = temp->weight;
                int v = temp->node;
                if (h[u] != inf && h[u] + w < h[v])
                {
                    h[v] = h[u] + w;
                    extra++;
                }
                temp = temp->next;
            }

            if (extra == 0)//if no adjustment was made
                break;
        }

        for (int u = 0; u <= n; u++)//checking for negative cycle
        {
            struct adj_list *temp;
            temp = arr[u];

            while (temp != NULL)
            {
                int w = temp->weight;
                int v = temp->node;
                if (h[u] != inf && h[u] + w < h[v])
                {
                    neg = 1;// if node were negative
                    break;
                }
                temp = temp->next;
            }

        }
    }
}

void johnson_algorithm(int n)
{
    int src = 0;//0 is taken as dummy node

    for (int i = 1; i <= n; i++)//adding edge from 0 to every node of weight 0
    {
        addEdges(0, i, 0);
    }

    bellman_ford(n, 0);//calling bellman ford

    if (neg)//if it contains a negative cycle
    {
        printf("Graph contains negative cycle\n");
        return;
    }

    for (int u = 1; u <= n; u++)//reweighting the edge
    {

        struct adj_list *temp;
        temp = arr[u];

        while (temp != NULL)
        {
            int w = temp->weight;
            int v = temp->node;
            temp->weight = w + h[u] - h[v];
            temp = temp->next;
        }
    }

    printf("Enter the following choices of heaps for Running Dijkstra Algorithm.\n");//choosing which heap to be used

    printf("1. Binomial Heap.\n2.Binary Heap.\n3.Fibonacci Heap.\n");
    int choice;
    scanf("%d", &choice);

    time_t start_t;
    struct timeval start, stop;
    double sec = 0;

    gettimeofday(&start, NULL); //for veryfying time of dijkstra run time

    if (choice == 1)
    {
        for (int i = 1; i <= n; i++)
            dijkstra_binomial(i, n); //calling dijjkstra for every node as source
        printf("Time taken for running on Binomial Heap\n");
    }
    else if (choice == 2)
    {
        for (int i = 1; i <= n; i++) {
            bin_N = 0;
            bin_dijkstra(i, n); //calling dijkstra using binary heap
        }
        printf("Time taken for running on Binary Heap\n");
    }
    else if (choice == 3)
    {
        for (int i = 1; i <= n; i++)
            fib_dijkstra(n, i);//calling dijkstra using fibonacci heap
        printf("Time taken for running on Fibonacci Heap\n");
    }
    else
    {
        printf("Invalid Choice\n");
    }


    gettimeofday(&stop, NULL);

    sec = (double)(stop.tv_usec - start.tv_usec) / 1000000 + (double)(stop.tv_sec - start.tv_sec);
    printf(" %f\n", sec); //printing the time taken for dijkstra algorithm

}



int main(int argc, char *argv[])
{
    bin_heap = malloc(5050 * sizeof(struct pair));
    bin_N = 0;

    for (int i = 0; i < minN; i++)
    {
        map[i] = -1;
    }

    printf("Enter number of nodes in a graph\n");
    int n;
    scanf("%d", &n);
    for (int i = 0; i < n; i++)
        arr[i] = NULL; //Initializing each node with NULL pointer (empty List)



    printf("Enter the type of graph. Tpye U if Graph is Undirected OR Type D if Graph is Directed.\n");
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
            scanf("%d%c%d%c%d%c", &u, &temp, &v, &temp, &w, &temp);//taking input for edges
            if (u > n || v > n || u <= 0 || v <= 0)
            {
                printf("Please enter Valid node\n");
                continue;
            }

            // add edges

            addEdges(u, v, w);
            addEdges(v, u, w);
        }


        johnson_algorithm(n);//calling johnson's algorithm

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
            addEdges(u, v, w);
        }


        johnson_algorithm(n);
    }
    else
    {
        printf("Invalid Choice\n");
    }

    return 0;
}
