#include <stdio.h>
#include <stdlib.h>

typedef int (*comp_keys)(int const *, int const *);

typedef enum status
{
    SUCCESS,
    MEMORY_ERROR,
    IDENTICAL_KEYS,
    MISSING
} status;

typedef struct Node
{
    int key;
    int data;
    struct Node *l, *r;
} Node;

typedef struct Tree
{
    Node *root;
    comp_keys comp;
} Tree;

int comparer(int const *key_1, int const *key_2)
{
    return *key_1 - *key_2;
}

void tree_init(Tree *tree, comp_keys cmp)
{
    if (!tree)
        return;
    tree->comp = cmp;
    tree->root = NULL;
}

status add_elem(Tree *tree, int key, int value)
{
    Node *new_node, **tmp_pptr;
    int comp_res;
    if (!tree)
        return MEMORY_ERROR;

    tmp_pptr = &tree->root;
    while (*tmp_pptr)
    {
        if ((comp_res = tree->comp(&key, &(*tmp_pptr)->key)) == 0)
        {
            return IDENTICAL_KEYS;
        }
        tmp_pptr = (comp_res < 0) ? &(*tmp_pptr)->l : &(*tmp_pptr)->r;
    }
    *tmp_pptr = (Node *)malloc(sizeof(Node));
    if (!*tmp_pptr)
        return MEMORY_ERROR;

    (*tmp_pptr)->l = (*tmp_pptr)->r = NULL;
    (*tmp_pptr)->data = value;
    (*tmp_pptr)->key = key;
    return SUCCESS;
}

status delete_elem(Tree *tree, int key)
{
    Node **tmp_pptr, *tmp_node, **successor;
    int comp_res;

    if (!tree)
        return MEMORY_ERROR;

    tmp_pptr = &tree->root;
    while (*tmp_pptr)
    {
        comp_res = tree->comp(&key, &(*tmp_pptr)->key);
        if (comp_res == 0)
            break;

        tmp_pptr = (comp_res < 0) ? &(*tmp_pptr)->l : &(*tmp_pptr)->r;
    }

    if (!*tmp_pptr)
        return SUCCESS;

    if ((*tmp_pptr)->l == NULL)
    {
        tmp_node = (*tmp_pptr)->r;
        free(*tmp_pptr);
        *tmp_pptr = tmp_node;
    }
    else if ((*tmp_pptr)->r == NULL)
    {
        tmp_node = (*tmp_pptr)->l;
        free(*tmp_pptr);
        *tmp_pptr = tmp_node;
    }
    else
    {
        successor = &(*tmp_pptr)->l;
        while ((*successor)->r)
        {
            successor = &(*successor)->r;
        }
        tmp_node = *successor;
        *successor = (*successor)->l;
        tmp_node->l = (*tmp_pptr)->l;
        tmp_node->r = (*tmp_pptr)->r;
        free(*tmp_pptr);
        *tmp_pptr = tmp_node;
    }
    return SUCCESS;
}

int *search_elem_kern(Node *root, comp_keys comp, int key)
{
    int res_comp;
    if (!root)
        return NULL;

    if ((res_comp = comp(&key, &root->key)) == 0)
        return &root->data;

    if (res_comp < 0)
        return search_elem_kern(root->l, comp, key);

    return search_elem_kern(root->r, comp, key);
}

status search_elem(Tree *tree, int key, int **data_ptr)
{
    if (!tree || !data_ptr)
        return MEMORY_ERROR;

    *data_ptr = search_elem_kern(tree->root, tree->comp, key);
    if (!*data_ptr)
        return MISSING;

    return SUCCESS;
}

void prefix(Node *root)
{
    if (!root)
        return;

    printf("%d ", root->key);
    prefix(root->l);
    prefix(root->r);
}

void delete_tree_kern(Node *root)
{
    if (!root)
        return;

    delete_tree_kern(root->l);
    delete_tree_kern(root->r);
    free(root);
}

void delete_tree(Tree *tree)
{
    if (!tree)
        return;

    delete_tree_kern(tree->root);
    tree->root = NULL;
    tree->comp = NULL;
}

void infix(Node *root)
{
    if (!root)
        return;

    infix(root->l);
    printf("%d ", root->key);
    infix(root->r);
}

void postfix(Node *root)
{
    if (!root)
        return;

    postfix(root->l);
    postfix(root->r);
    printf("%d ", root->key);
}

int main()
{
    Tree tree;
    tree_init(&tree, comparer);
    add_elem(&tree, 1, 100);
    add_elem(&tree, 2, 200);
    add_elem(&tree, 3, 200);
    add_elem(&tree, -100, 200);
    add_elem(&tree, -1, 200);
    add_elem(&tree, -200, 200);
    prefix(tree.root);
    putchar('\n');
    infix(tree.root);
    putchar('\n');
    postfix(tree.root);

    int *ptr;
    search_elem(&tree, 1, &ptr);
    delete_tree(&tree);
    return 0;
}