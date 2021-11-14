#include<stdio.h>
#include<stdlib.h>

struct node {
	int value, weight;
}
typedef node;

struct binomialNode {
	node key;
	int degree;
	struct binomialNode* parent , *child, *sibling;
}
typedef binomialNode;
binomialNode * binomialHead = NULL;

binomialNode* newbinomialNode(void) {
	binomialNode* h1  = (binomialNode*)malloc(sizeof(binomialNode));
	h1->degree = 0, h1->parent = h1->child = h1->sibling = NULL;
	return h1;
}

binomialNode* getMinBinomialHeap(void) {
	binomialNode* curr =  binomialHead, *result;
	result = curr;
	if (result == NULL) return result;
	node min = curr->key;
	while (curr) {
		if (curr->key.weight < min.weight) {
			min = curr->key;
			result = curr;
		}
		curr = curr->sibling;
	}
	return result;
}
binomialNode* unionBinomialHeap(binomialNode* h1, binomialNode *h2) {
	binomialNode* newHeap = NULL, *curr1 = h1, *curr2 = h2;
	binomialNode* curr3 = newHeap;
	while (curr1 && curr2) {
		if (curr1->degree <= curr2->degree) {
			if (curr3 ==	NULL) {
				curr3 = newHeap = curr1;
			}
			else curr3->sibling = curr1;
			curr1 = curr1->sibling;
		}
		else {
			if (curr3 ==	NULL) {
				curr3 = newHeap = curr2;
			}
			else curr3->sibling = curr2;
			curr2 = curr2->sibling;
		}
		curr3 = curr3->sibling;
	}
	while (curr1) {
		curr3->sibling = curr1;
		curr1 = curr1->sibling;
		curr3 = curr3->sibling;
	}
	while (curr2) {
		curr3->sibling = curr2;
		curr1 = curr1->sibling;
		curr3 = curr3->sibling;
	}
	return newHeap;
}
void mergeBinomialHeap(void) {
	if (binomialHead == NULL)return;
	binomialNode* prev = NULL, *ptr = binomialHead, *next = binomialHead->sibling;

}
void insertBinomial(node x) {
	binomialNode* h1 = newbinomialNode();
	h1->key = x;
	binomialHead = unionBinomialHeap(h1, binomialHead);
	mergeBinomialHeap();
}
int main() {
	return 0;
}