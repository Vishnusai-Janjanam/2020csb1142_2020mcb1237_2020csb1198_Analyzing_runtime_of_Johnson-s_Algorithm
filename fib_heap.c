#include<stdio.h>
#include<malloc.h>
#include<stdbool.h>
#include<stdlib.h>

struct node_fib{
    struct node_fib * parent;
    struct node_fib * child;
    struct node_fib * left;
    struct node_fib * right;
    int key;
    int degree;
    char mark;
    char c;
};


void insertion(struct node_fib** root,int *number_nodes_fib,int val)
{
    struct node_fib * temp=(struct node_fib*)malloc(sizeof(struct node_fib));
    temp->key=val;
    temp->degree=0;
    temp->mark='W';
    temp->c='N';
    temp->parent=NULL;
    temp->child=NULL;
    temp->left=temp;
    temp->right=temp;

    if(*root!=NULL)
    {
        (*root)->left->right=temp;
        temp->right=*root;
        temp->left=(*root)->left ;
        (*root)->left=temp;
    
        if(temp->key<(*root)->key)
        {
            (*root)=temp;
        }
    }
    else {
        (*root)=temp;
    }
    *number_nodes_fib=*number_nodes_fib+1;
}



void create_fibonacci_link(struct node_fib ** root,struct node_fib *ptr2, struct node_fib * ptr1)
{
    ptr2->left->right=ptr2->right;
    ptr2->right->left=ptr2->left;


    if(ptr1->right==ptr1)
    {
        (*root)=ptr1;
    }


    ptr2->left=ptr2;
    ptr2->right=ptr2;
    ptr2->parent=ptr1;


    if(ptr1->child==NULL)
    {
        ptr1->child=ptr2;
    }

    ptr2->right=ptr1->child;
    ptr2->left=ptr1->child->left;

    ptr1->child->left->right=ptr2;
    ptr1->child->left=ptr2;

    if(ptr2->key<ptr1->child->key)
    ptr1->child=ptr2;
    ptr1->degree=ptr1->degree+1;
}



void consolidate(struct node_fib ** root,int *number_nodes_fib)
{
    int temp1;
    float temp2=(log(*number_nodes_fib)/log(2));
    int temp3=temp2;

    struct node_fib * arr[temp3+1];

    for(int i=0;i<=temp3;i++){
        arr[i]=NULL;
    }


    struct node_fib *ptr1=NULL;
    struct node_fib *ptr2;
    struct node_fib *ptr3;
    struct node_fib *ptr4=ptr1;

    do{
        ptr4 = ptr4->right;
        temp1 = ptr1->degree;
        while (arr[temp1] != NULL) {
            ptr2 = arr[temp1];
            if (ptr1->key > ptr2->key) {
                ptr3 = ptr1;
                ptr1 = ptr2;
                ptr2 = ptr3;
            }
            if (ptr2 == (*root))
             (*root) = ptr1;
            Fibonnaci_link(ptr2, ptr1);
            if (ptr1->right == ptr1)
                (*root) = ptr1;
            arr[temp1] = NULL;
            temp1++;
        }
        arr[temp1] = ptr1;
        ptr1 = ptr1->right;
    } while (ptr1 != (*root));
 (*root) = NULL;
    for (int j = 0; j <= temp3; j++) {
        if (arr[j] != NULL) {
            arr[j]->left = arr[j];
            arr[j]->right = arr[j];
            if  ((*root) != NULL) {
                (*root)->left->right=arr[j];
                arr[j]->right = (*root);
                arr[j]->left = (*root)->left;
             (*root)->left = arr[j];
                if (arr[j]->key < (*root)->key)
                 (*root) = arr[j];
            }
            else {
             (*root) = arr[j];
            }
            if  ((*root) == NULL)
             (*root) = arr[j];
            else if (arr[j]->key < (*root)->key)
             (*root) = arr[j];
        }
    }
    
}



void Extract_min(struct node_fib **root,int *number_nodes_fib)
{
    if (*root == NULL)
        {
            return;
        }    
    else {
        struct node_fib* temp = (*root);
        struct node_fib* pntr;
        pntr = temp;
        struct node_fib* x = NULL;
        if (temp->child != NULL) {
 
            x = temp->child;
            do {
                pntr = x->right;
                ((*root)->left)->right = x;
                x->right = (*root);
                x->left = (*root)->left;
                (*root)->left = x;
                if (x->key < (*root)->key)
                    (*root)= x;
                x->parent = NULL;
                x = pntr;
            } while (pntr != temp->child);
        }
        (temp->left)->right = temp->right;
        (temp->right)->left = temp->left;
        (*root) = temp->right;
        if (temp == temp->right && temp->child == NULL)
            (*root) = NULL;
        else {
            (*root) = temp->right;
            Consolidate();
        }
        *number_nodes_fib=*number_nodes_fib+1;
    }
}

void Cut(struct node_fib ** root,struct node_fib* found, struct node_fib* temp)
{
    if (found == found->right)
        temp->child = NULL;
 
    (found->left)->right = found->right;
    (found->right)->left = found->left;
    if (found == temp->child)
        temp->child = found->right;
 
    temp->degree = temp->degree - 1;
    found->right = found;
    found->left = found;
    ((*root)->left)->right = found;
    found->right = (*root);
    found->left = (*root)->left;
    (*root)->left = found;
    found->parent = NULL;
    found->mark = 'B';
}


void Cascase_cut(struct node_fib ** root,struct node_fib* temp)
{
    struct node_fib* ptr5 = temp->parent;
    if (ptr5 != NULL) {
        if (temp->mark == 'W') {
            temp->mark = 'B';
        }
        else {
            Cut(root,temp, ptr5);
            Cascase_cut(root,ptr5);
        }
    }
}


void Decrease_key(struct node_fib ** root,struct node_fib* found, int val)
{
    if ((*root) == NULL)
        return;
    if (found == NULL)
        return;
    found->key = val;
 
    struct node_fib* temp = found->parent;
    if (temp != NULL && found->key < temp->key) {
        Cut(root,found, temp);
        Cascase_cut(root,temp);
    }
    if (found->key < (*root)->key)
        (*root) = found;
}

void Find(struct node_fib** root,struct node_fib* mini, int old_val, int val)
{
    struct node_fib* found = NULL;
    struct node_fib* temp5 = mini;
    temp5->c = 'Y';
    struct node_fib* found_ptr = NULL;
    if (temp5->key == old_val) {
        found_ptr = temp5;
        temp5->c = 'N';
        found = found_ptr;
        Decrease_key(root,found, val);
    }
    if (found_ptr == NULL) {
        if (temp5->child != NULL)
            Find(root,temp5->child, old_val, val);
        if ((temp5->right)->c != 'Y')
            Find(root,temp5->right, old_val, val);
    }
    temp5->c = 'N';
    found = found_ptr;
}


void Deletion(struct node_fib** root,int * number_nodes_fib,int val)
{
    if ((*root) == NULL)
        return;
    else {
        Find(root,(*root), val, 0);
 
        Extract_min(root,number_nodes_fib);
    }
}

