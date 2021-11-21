struct node {									//This stores the pair vertex and weight
	int vertex, weight;
}
typedef node;
//const int inf = 107374;
struct binomialNode {		// This is the node of the binomial heap
	node key;
	int degree;
	struct binomialNode* parent , *fc, *lc, *ls, *rs;
}
// fc means the first child, lc means the last child, ls means the left sibling and rs means the right sibling
typedef binomialNode;
binomialNode *binomialHead = NULL;
binomialNode** map;
binomialNode* new_binomialNode(void) {			// This function creates a new binomial node and initiaizes the degree to 0 and all pointers to NULL.
	binomialNode* h1  = (binomialNode*)malloc(sizeof(binomialNode));
	h1->degree = 0, h1->parent = h1->lc = h1->rs = h1->ls = h1->fc = NULL;
	return h1;
}

binomialNode* get_min_binomial_heap(void) {		//this function finds the minimum element in the heap
	binomialNode* curr =  binomialHead, *result;
	result = curr;
	if (result == NULL) return result;
	node min = curr->key;
	while (curr) {										//Searching all the roots(as the roots contain the minimum element of the respective tree)
		if (curr->key.weight < min.weight) {
			min = curr->key;
			result = curr;
		}
		curr = curr->rs;
	}
	return result;
}
binomialNode* union_binomial_heap(binomialNode* h1, binomialNode *h2) {			// This function unites two binomial heaps
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
binomialNode* merge_binomial_tree(binomialNode*t1, binomialNode* t2) { 		//This function merges two binomial nodes with same degree
	if (t1->key.weight < t2->key.weight) { // When prev key is smaller then the ptr key.
		binomialNode* temp = t1->lc;	//Storing the last child of the prev node.
		t1->lc = t2;		//Updating the last child of prev node
		(t1->degree)++;		//Updating the degree of ptr node.
		t2->parent = t1;	//Adding parent of ptr node
		t2->rs = temp;		//making temp the right sibling of ptr node.
		if (temp) temp->ls = t2;
		if (t1->degree == 1) t1->fc = t2; if //If t2 is the first child.
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
void merge_binomial_heap(void) {		//This function will merge all the heaps with same degree which are present initially and during the process too.
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
		else {														//degree of ptr == degree of next.
			if (prev == NULL) {								//When first two node are to be merged
				binomialNode* t1 = nextn;
				ptr = merge_binomial_tree(ptr, next);
				binomialHead  = ptr;
				ptr->rs = t1;
				next = t1;
				if (next) nextn = next->rs;
			}
			else {												//if merging nodes are somewhere in between
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
void insert_binomial_node(node x) {		//Function to insert a binomial node.
	binomialNode* h1 = new_binomialNode();
	h1->key = x;
	map[x.vertex] = h1;
	binomialHead = union_binomial_heap(h1, binomialHead);	//Taking union of the two heaps
	merge_binomial_heap();				//Performing the merge operation
}
void delete_min_binomial(void) {				//Function to delete the min node of the binomial heap
	binomialNode* min_node = get_min_binomial_heap();
	binomialNode* curr = binomialHead, *prev = NULL, *temp_h1;
	while (curr) {	//Finding the prev node of min node.
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

		map[pos->key.vertex] = par;

		pos->key.vertex = par_vertex;
		pos->key.weight = par_weight;
		map[par_vertex] = pos;
		//going up the tree in such a way that binary heap property is maintained.
		pos = par;
		par = pos->parent;
	}
}
void dijkstra_binomial(int s, int n) {//Dijsktra using binomial heap
	node temp;
	map = (binomialNode**) malloc(sizeof(binomialNode*) * (n + 1));//Stores the position of each vertex in the binomial heap
	int vst[n + 1], dis[n + 1];		//visit and distance array
	temp.vertex = s, temp.weight = 0;
	insert_binomial_node(temp);//inserting the  source node.
	for (int i = 1; i <= n; i++) {
		map[i] = NULL;
		vst[i] = 0;
		dis[i] = inf;
		if (i != s) {
			temp.vertex = i;
			temp.weight = inf;
			insert_binomial_node(temp);		// Adding all other nodes with best distance as inf
		}
	}
	dis[s] = 0;
	while (binomialHead) {
		binomialNode* min_node = get_min_binomial_heap();//Finding the min node
		delete_min_binomial();		//Deleting the min node
		int u = min_node->key.vertex;
		if (vst[u]) continue;
		vst[u] = 1;
		struct adj_list* curr = arr[u];
		while (curr) {		//traversing the adjacency list of the the vertex u.
			if (dis[curr->node] > dis[u] + curr->weight) {
				dis[curr->node] = dis[u] = curr->weight;
				decrease_key_binomial_heap(map[curr->node], dis[curr->node]); //decreasing the key.
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