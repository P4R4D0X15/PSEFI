#ifndef COMMON_H
#define COMMON_H
#include "common.h"

#define FIFO_SIZE 10

typedef struct queue {
    size_t size;
    size_t tail;
    filter_t aref[FIFO_SIZE];
    size_t head;
} queue;

extern void queue_empty(queue *q);

extern int queue_put(queue *q, filter_t ref);

extern filter_t queue_pop(queue *q);

#endif
