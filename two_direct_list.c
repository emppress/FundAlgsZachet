#include "stdio.h"
#include "stdlib.h"

typedef enum status
{
    SUCCESS,
    MEMORY_ERROR,
    INDEX_ERROR,
    MISSING
} status;

typedef struct Node
{
    int data;
    struct Node *next, *prev;
} Node;

typedef struct List
{
    Node *head;
} List;

void list_init(List *list)
{
    if (!list)
        return;

    list->head = NULL;
}

status list_insert(List *list, size_t indx, int data)
{
    Node *new_node, *tmp;
    size_t i;
    if (!list)
        return MEMORY_ERROR;

    new_node = (Node *)malloc(sizeof(Node));
    if (!new_node)
        return MEMORY_ERROR;

    new_node->next = new_node->prev = NULL;
    new_node->data = data;
    if (!list->head || indx == 0)
    {
        if (list->head)
        {
            list->head->prev = new_node;
        }
        new_node->next = list->head;
        list->head = new_node;
        return SUCCESS;
    }

    tmp = list->head;
    for (i = 0; i < indx - 1; ++i)
    {
        if (tmp->next)
            tmp = tmp->next;
    }

    new_node->next = tmp->next;
    new_node->prev = tmp;
    if (tmp->next)
        tmp->next->prev = new_node;
    tmp->next = new_node;
    return SUCCESS;
}

status list_delete_elem(List *list, size_t indx)
{
    Node *next, *tmp;
    size_t i;
    if (!list)
        return MEMORY_ERROR;

    if (!list->head)
        return INDEX_ERROR;

    if (indx == 0)
    {
        next = list->head->next;
        free(list->head);
        list->head = next;
        next->prev = NULL;
        return SUCCESS;
    }

    tmp = list->head;
    for (i = 0; i < indx; ++i)
    {
        if (!tmp->next)
            return INDEX_ERROR;
        tmp = tmp->next;
    }

    tmp->prev->next = tmp->next;
    tmp->next->prev = tmp->prev;
    free(tmp->next);

    return SUCCESS;
}

status get_elem_indx(List *list, size_t indx, int **found)
{
    Node *tmp;
    size_t i;
    if (!list || !found)
        return MEMORY_ERROR;

    if (!list->head)
        return MEMORY_ERROR;

    tmp = list->head;
    for (i = 0; i < indx; ++i)
    {
        if (!tmp->next)
            return INDEX_ERROR;

        tmp = tmp->next;
    }
    *found = &tmp->data;
    return SUCCESS;
}

status get_elem_value(List *list, int value, int **found)
{
    Node *tmp;
    if (!list || !found)
        return MEMORY_ERROR;

    if (!list->head)
        return MEMORY_ERROR;

    tmp = list->head;
    while (tmp)
    {
        if (tmp->data == value)
        {
            *found = &tmp->data;
            return SUCCESS;
        }
        tmp = tmp->next;
    }
    return MISSING;
}

int main()
{
    List list;
    int *elem;
    list_init(&list);
    list_insert(&list, 0, 0);
    list_insert(&list, 1, 1);
    list_insert(&list, 2, 2);
    list_insert(&list, 3, 3);
    list_insert(&list, 100, 4);
    list_insert(&list, 2, 9999);
}