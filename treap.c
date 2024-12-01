#include <stdio.h>
#include <stdlib.h>
#include <time.h>

typedef int (*cmp_type)(const int *, const int *);

typedef struct Node
{
    size_t prior;
    int key;
    struct Node *l, *r;
} Node;

typedef struct Heap
{
    Node *root;
    cmp_type cmp_keys;
} Heap;

int comparator(const int *a, const int *b)
{
    return *a - *b;
}

void swap(Node **n_1, Node **n_2)
{
    Node *tmp = *n_1;
    *n_1 = *n_2;
    *n_2 = tmp;
}

void init_heap(Heap *heap, cmp_type cmp)
{
    if (!heap)
        return;

    heap->root = NULL;
    heap->cmp_keys = cmp;
}

Node *merge_kernel(Node *root_1, Node *root_2) // ключи root_1 <= ключей root_2
{
    if (!root_1)
        return root_2;
    if (!root_2)
        return root_1;

    if (root_1->prior <= root_2->prior)
    {
        root_1->r = merge_kernel(root_1->r, root_2);
        return root_1;
    }
    else
    {
        root_2->l = merge_kernel(root_2->l, root_1);
        return root_2;
    }
}

void split(Node *root, int key, Node **left, Node **right, cmp_type keys_comp)
{
    if (!root)
    {
        *left = *right = NULL;
        return;
    }

    if (keys_comp(&key, &root->key) < 0)
    {
        split(root->l, key, left, &root->l, keys_comp);
        *right = root;
    }
    else
    {
        split(root->r, key, &root->r, right, keys_comp);
        *left = root;
    }
}

Node *create_node(int key, size_t priority)
{
    Node *node = (Node *)malloc(sizeof(Node));
    if (!node)
    {
        return NULL;
    }
    node->key = key;
    node->prior = priority;
    node->l = node->r = NULL;
    return node;
}

void insert(Node **root, int key, size_t priority, cmp_type comp_keys)
{
    Node *new_node;
    if (!root)
    {
        *root = create_node(key, priority);
        return;
    }

    if (priority < (*root)->prior)
    {
        new_node = create_node(key, priority);
        split(*root, key, &new_node->l, &new_node->r, comp_keys);
        *root = new_node;
    }
    else if (comp_keys(&key, &(*root)->key) < 0)
    {
        insert((*root)->l, key, priority, comp_keys);
    }
    else
        insert((*root)->r, key, priority, comp_keys);
}

void erase(Node **root, int key, cmp_type comp_keys)
{
    if (!*root)
        return;

    if ((*root)->key == key)
    {
        Node *temp = merge_kernel((*root)->l, (*root)->r);
        free(*root);
        *root = temp;
    }
    else if (key < (*root)->key)
        erase(&(*root)->l, key, comp_keys);
    else
        erase(&(*root)->r, key, comp_keys);
}
