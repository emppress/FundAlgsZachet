#include <stdio.h>
#include "stdlib.h"

#define min(a, b) (((a) < (b)) ? (a) : (b))

typedef int (*comp_keys)(int const *, int const *);

typedef struct Node
{
    int key, data;
    size_t rank;
    struct Node *l, *r;
} Node;

typedef struct Heap
{
    Node *root;
    comp_keys cmp;
} Heap;

int comparer(int const *key_1, int const *key_2)
{
    return *key_1 - *key_2;
}

void init_heap(Heap *heap, comp_keys cmp)
{
    if (!heap)
        return;

    heap->cmp = cmp;
    heap->root = NULL;
}

void destroy_heap_kern(Node *root)
{
    if (!root)
        return;

    destroy_heap_kern(root->l);
    destroy_heap_kern(root->r);
    free(root);
}

void destroy_heap(Heap *heap)
{
    if (!heap)
        return;

    destroy_heap_kern(heap->root);
    heap->root = NULL;
    heap->cmp = NULL;
}

void swap(Node **n_1, Node **n_2)
{
    Node *tmp;
    tmp = *n_1;
    *n_1 = *n_2;
    *n_2 = tmp;
}

Node *merge_kernel(Node *root_1, Node *root_2, comp_keys cmp)
{
    if (root_1 == NULL)
        return root_2;

    if (root_2 == NULL)
        return root_1;

    if (cmp(&root_1->key, &root_2->key) > 0)
        swap(&root_1, &root_2);

    root_1->r = merge_kernel(root_1->r, root_2, cmp);

    if (root_1->l == NULL || root_1->l->rank < root_1->r->rank)
    {
        swap(&root_1->l, &root_1->r);
    }

    root_1->rank = (root_1->r) ? min(root_1->l->rank, root_1->r->rank) + 1 : 1;
    return root_1;
}

// разрушением будет являться запись результата в heap_1
// и обнуление heap_2
void merge_with_destroy(Heap *heap_1, Heap *heap_2)
{

    if (!heap_1 || !heap_2)
        return;

    heap_1->root = merge_kernel(heap_1->root, heap_2->root, heap_1->cmp);
    heap_2->cmp = NULL;
    heap_2->root = NULL;
}

Node *create_node(int data, int key)
{
    Node *res = (Node *)malloc(sizeof(Node));
    if (!res)
        return NULL;

    res->data = data;
    res->key = key;
    res->l = res->r = NULL;
    res->rank = 1;
}

void add_elem(Heap *heap, int data, int key)
{
    Heap atomic_heap;
    if (!heap)
        return;

    init_heap(&atomic_heap, heap->cmp);
    atomic_heap.root = create_node(data, key);
    if (!atomic_heap.root)
        return;

    merge_with_destroy(heap, &atomic_heap);
}

void print_heap(Node *root, int shift)
{
    if (!root)
        return;

    print_heap(root->r, shift + 1);
    for (int i = 0; i < shift * 2; ++i)
    {
        putchar(' ');
    }
    printf("%d\n", root->key);
    print_heap(root->l, shift + 1);
}

void pop_first(Heap *heap, int *data, int *key) // тут вместо войда ерроры бы прокинуть...
{
    Heap lheap, rheap;
    if (!heap || !data || !key || !heap->root)
        return;

    init_heap(&lheap, heap->cmp);
    init_heap(&rheap, heap->cmp);
    lheap.root = heap->root->l;
    rheap.root = heap->root->r;
    *data = heap->root->data;
    *key = heap->root->key;
    merge_with_destroy(&lheap, &rheap);
    free(heap->root);
    heap->root = lheap.root;
}

int main()
{
    Heap heap_1, heap_2;
    int data, key;
    init_heap(&heap_1, comparer);
    init_heap(&heap_2, comparer);

    add_elem(&heap_1, 6, 6);
    add_elem(&heap_1, 7, 7);
    add_elem(&heap_1, 12, 12);
    add_elem(&heap_1, 18, 18);
    add_elem(&heap_1, 24, 24);
    add_elem(&heap_1, 8, 8);
    add_elem(&heap_1, 37, 37);
    add_elem(&heap_1, 33, 33);
    add_elem(&heap_1, 17, 17);
    add_elem(&heap_1, 18, 18);
    add_elem(&heap_1, 26, 26);

    add_elem(&heap_2, 10, 10);
    add_elem(&heap_2, 21, 21);
    add_elem(&heap_2, 14, 14);
    add_elem(&heap_2, 23, 23);

    print_heap(heap_1.root, 0);
    putchar('\n');
    putchar('\n');
    putchar('\n');
    print_heap(heap_2.root, 0);

    Node *res;
    merge_with_destroy(&heap_1, &heap_2);
    putchar('\n');
    putchar('\n');
    putchar('\n');
    print_heap(heap_1.root, 0);

    pop_first(&heap_1, &data, &key);
    putchar('\n');
    putchar('\n');
    putchar('\n');
    print_heap(heap_1.root, 0);

    pop_first(&heap_1, &data, &key);
    putchar('\n');
    putchar('\n');
    putchar('\n');
    print_heap(heap_1.root, 0);

    pop_first(&heap_1, &data, &key);
    putchar('\n');
    putchar('\n');
    putchar('\n');
    print_heap(heap_1.root, 0);
    destroy_heap(&heap_1);

    return 0;
}