#include "stdio.h"
#include <stdlib.h>

typedef struct node
{
    int data;
    struct node *next;
} node;

typedef struct queue
{
    node *head, *tail;
} queue;

void init(queue *q)
{
    if (!q)
        return;
    q->head = q->tail = NULL;
}

int is_empty(queue *q)
{
    if (!q)
        return -1;

    if (!q->head)
        return 1;

    return 0;
}

int push(queue *q, int data)
{
    if (!q)
        return -1;

    node *new_node = (node *)malloc(sizeof(node));
    if (!new_node)
        return -1;

    new_node->data = data;
    new_node->next = NULL;

    if (is_empty(q))
    {
        q->head = q->tail = new_node;
    }
    else
    {
        q->tail->next = new_node;
        q->tail = new_node;
    }
    return 0;
}

int test(queue *q, int *data)
{
    if (!q || !data)
        return -1;

    if (is_empty(q))
        return -2;

    *data = q->head->data;
    return 0;
}

int pop(queue *q, int *data)
{
    node *tmp;
    if (!q || !data)
        return -1;

    if (is_empty(q))
        return -2;

    *data = q->head->data;
    tmp = q->head;
    q->head = tmp->next;
    free(tmp);
    return 0;
}

void destroy(queue *q)
{
    if (!q)
        return;

    node *cur, *next;
    cur = q->head;
    while (cur)
    {
        next = cur->next;
        free(cur);
        cur = next;
    }
    q->head = q->tail = NULL;
}

int main()
{
    queue q;
    int data;
    init(&q);
    push(&q, 1);
    push(&q, 2);
    push(&q, 3);
    push(&q, 4);
    push(&q, 5);

    pop(&q, &data);
    printf("%d", data);
    pop(&q, &data);
    printf("%d", data);
    pop(&q, &data);
    printf("%d", data);
    pop(&q, &data);
    printf("%d", data);
    pop(&q, &data);
    printf("%d", data);

    push(&q, 10);
    destroy(&q);
    return 0;
}