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
    struct Node *next;
} Node;

typedef struct List
{
    Node *head;
    size_t size;
} List;

void list_init(List *list)
{
    if (!list)
        return;

    list->head = NULL;
    list->size = 0;
}

status list_insert(List *list, size_t indx, int data)
{
    Node *new_node, *tmp;
    size_t i;
    if (!list)
        return MEMORY_ERROR;

    new_node = (Node *)malloc(sizeof(Node)); // 0 - 0 - 0 - 0 - 0 - 0 - 0
    if (!new_node)
        return MEMORY_ERROR;

    new_node->next = NULL;
    new_node->data = data;
    if (!list->head)
    {
        new_node->next = new_node;
        list->head = new_node;
        ++list->size;
        return SUCCESS;
    }

    if (indx == 0)
    {
        new_node->next = list->head;

        tmp = list->head;
        while (tmp->next != list->head)
        {
            tmp = tmp->next;
        }
        tmp->next = new_node;
        list->head = new_node;
        ++list->size;
        return SUCCESS;
    }

    indx = (indx % list->size) ? indx % list->size : list->size;

    tmp = list->head;
    for (i = 0; i < indx - 1; ++i)
    {
        tmp = tmp->next;
    }

    new_node->next = tmp->next;
    tmp->next = new_node;
    ++list->size;
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

    indx %= list->size;
    if (indx == 0)
    {
        next = list->head->next;

        tmp = list->head;
        while (tmp->next != list->head)
        {
            tmp = tmp->next;
        }
        tmp->next = next;
        free(list->head);
        list->head = next;
        if (list->size == 1)
            list->head = NULL;
        --list->size;
        return SUCCESS;
    }

    tmp = list->head;
    for (i = 0; i < indx - 1; ++i)
    {
        if (!tmp->next)
            return INDEX_ERROR;
        tmp = tmp->next;
    }

    if (!tmp->next)
        return INDEX_ERROR;

    next = tmp->next->next;
    free(tmp->next);
    tmp->next = next;
    --list->size;
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

    indx %= list->size;
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
        if (tmp->next == list->head)
            break;
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
    list_insert(&list, 0, 999);
    get_elem_indx(&list, 4, &elem);
    get_elem_indx(&list, 5, &elem);
    get_elem_indx(&list, 0, &elem);
    get_elem_indx(&list, 12, &elem);
    get_elem_value(&list, 312321, &elem);
    get_elem_value(&list, 0, &elem);
}