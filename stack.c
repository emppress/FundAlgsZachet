#include "stdio.h"
#include <stdlib.h>

typedef struct node
{
    int data;
    struct node *next;
} node;

typedef struct stack
{
    node *head;
} stack;

void init(stack *st)
{
    if (st)
        st->head = NULL;
}

int push(stack *st, int data)
{
    if (!st)
        return -1;

    node *new_node = (node *)malloc(sizeof(node));
    if (!new_node)
        return -1;

    new_node->data = data;
    new_node->next = st->head;
    st->head = new_node;
    return 0;
}

int pop(stack *st, int *data)
{
    if (!st || !data)
        return -1;

    if (!st->head)
        return -2;

    *data = st->head->data;
    node *temp = st->head;
    st->head = temp->next;
    free(temp);
    return 0;
}

int is_empty(stack *st)
{
    if (!st)
        return -1;

    if (st->head)
        return 0;

    return 1;
}

int peak(stack *st, int *data)
{
    if (!st || !data)
        return -1;

    if (is_empty(st))
        return -2;

    *data = st->head->data;
    return 0;
}

void destroy(stack *st)
{
    if (!st)
        return;

    node *cur, *next;
    cur = st->head;
    while (cur)
    {
        next = cur->next;
        free(cur);
        cur = next;
    }
}

int main()
{
    stack st;
    int data;

    init(&st);
    push(&st, 1);
    push(&st, 2);
    push(&st, 3);
    push(&st, 4);
    push(&st, 5);
    pop(&st, &data);
    printf("%d", data);
    pop(&st, &data);
    printf("%d", data);
    pop(&st, &data);
    printf("%d", data);
    pop(&st, &data);
    printf("%d", data);
    pop(&st, &data);
    printf("%d", data);
    destroy(&st);
    return 0;
}