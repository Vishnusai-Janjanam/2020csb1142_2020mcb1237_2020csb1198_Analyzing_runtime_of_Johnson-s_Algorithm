#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>
struct node {
	int vertex, weight;
}
typedef node;
//const int inf = 1073741824;
struct binomialNode {
	node key;
	int degree;
	struct binomialNode* parent , *fc, *lc, *ls, *rs;
}
typedef binomialNode;
binomialNode *binomialHead = NULL;

binomialNode* new_binomialNode(void) {
	binomialNode* h1  = (binomialNode*)malloc(sizeof(binomialNode));
	h1->degree = 0, h1->parent = h1->lc = h1->rs = h1->ls = h1->fc = NULL;
	return h1;
}

binomialNode* get_min_binomial_heap(void) {
	binomialNode* curr =  binomialHead, *result;
	result = curr;
	if (result == NULL) return result;
	node min = curr->key;
	while (curr) {
		if (curr->key.weight < min.weight) {
			min = curr->key;
			result = curr;
		}
		curr = curr->rs;
	}
	return result;
}
binomialNode* union_binomial_heap(binomialNode* h1, binomialNode *h2) {
	binomialNode* newHeap = NULL, *curr1 = h1, *curr2 = h2;
	binomialNode* curr3 = newHeap;
	while (curr1 && curr2) {
		if (curr1->degree <= curr2->degree) {
			if (curr3 ==	NULL) {
				curr3 = newHeap = curr1;
			}
			else curr3->rs = curr1;
			curr1 = curr1->rs;
		}
		else {
			if (curr3 ==	NULL) {
				curr3 = newHeap = curr2;
			}
			else curr3->rs = curr2;
			curr2 = curr2->rs;
		}
		curr3 = curr3->rs;
	}
	while (curr1) {
		curr3->rs = curr1;
		curr1 = curr1->rs;
		curr3 = curr3->rs;
	}
	while (curr2) {
		curr3->rs = curr2;
		curr1 = curr1->rs;
		curr3 = curr3->rs;
	}
	return newHeap;
}
binomialNode* merge_binomial_tree(binomialNode*t1, binomialNode* t2) {
	if (t1->key.weight < t2->key.weight) {
		binomialNode* temp = t1->lc;
		t1->lc = t2;
		(t1->degree)++;
		t2->parent = t1;
		t2->rs = temp;
		if (temp) temp->ls = t2;
		if (t1->degree == 1) t1->fc = t2;
		return t1;
	}
	binomialNode* temp = t1->lc;
	(t2->degree)++;
	t1->parent = t2;
	t2->lc = t1;
	t1->rs = temp;
	if (temp) temp->ls = t1;
	if (t2->degree == 1) t2->fc = t1;
	return t2;
}
void merge_binomial_heap(void) {
	if (binomialHead == NULL || binomialHead->rs == NULL)return;
	binomialNode *ptr = binomialHead, *next = ptr->rs, *nextn = next->rs;
	while (next) {
		if (next->degree != ptr->degree) {
			ptr = next;
			next = nextn;
			if (nextn)nextn = nextn->rs;
		}
		else if (nextn && nextn->degree == next->degree) {
			ptr = next;
			next = nextn;
			nextn = nextn->rs;
		}
		else ptr = merge_binomial_tree(ptr, next);
	}

}
void insert_binomial_node(node x, binomialNode***a) {
	binomialNode* h1 = new_binomialNode();
	h1->key = x;
	*a[x.vertex] = h1;
	binomialHead = union_binomial_heap(h1, binomialHead);
	merge_binomial_heap();
}
void delete_min_binomial(void) {
	binomialNode* min = get_min_binomial_heap();
	binomialNode* temp_h1 = min->fc, *curr = temp_h1;
	while (curr) {
		curr->parent = NULL;
		curr->ls = curr->rs;
		curr->ls = NULL;
		curr = curr->rs;
	}
	union_binomial_heap(binomialHead, temp_h1);
	merge_binomial_heap();
}
void decrease_key_binomial_heap(binomialNode* pos, int new_key, binomialNode***a) {
	pos->key.weight = new_key;
	binomialNode* par = pos->parent;
	while (par && par->key.weight > pos->key.weight) {
		int par_vertex = par->key.vertex;
		int par_weight = par->key.weight;
		par ->key.vertex = pos->key.vertex;
		par->key.weight = pos->key.weight;
		*a[pos->key.vertex] = par;
		pos->key.vertex = par_vertex;
		pos->key.weight = par_weight;
		*a[par_vertex] = pos;
		pos = par;
		par = pos->parent;
	}
}
void dijkstra_binomial(int s, int n) {
	node temp;
	binomialNode** map;
	map = (binomialNode**) malloc(sizeof(binomialNode*) * (n + 1));
	int vst[n + 1], dis[n + 1];
	temp.vertex = s, temp.weight = inf;
	insert_binomial_node(temp, &map);
	for (int i = 1; i <= n; i++) {
		vst[i] = 0;
		dis[i] = inf;
		if (i != s) {
			temp.vertex = i;
			temp.weight = inf;
			insert_binomial_node(temp, &map);
		}
	}
	dis[s] = 0;
	while (binomialHead) {
		binomialNode* min_node = get_min_binomial_heap();
		delete_min_binomial();
		int u = min_node->key.vertex;
		if (vst[u]) continue;
		vst[u] = 1;
		for (int i = 1; i <= n; i++) {
			if (adj[u][i]) {
				if (dis[i] > dis[u] + w[u][i]) {
					dis[i] = dis[u] = w[u][i];
					decrease_key_binomial_heap(map[i], dis[i], &map);
				}
			}
		}
	}
	for (int i = 1; i <= n; i++)
	{
		if (dis[i] == inf)
			printf("Vertex %d is NOT reachable from source %d.\n", i, s);
		else
			printf("Cost of reaching vertex %d from source %d is %d.\n", i, s, dis[i] + h[i] - h[s]);

	}
}