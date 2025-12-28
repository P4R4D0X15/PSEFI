#include "queue.h"
#include "../error/error.h"
#include <semaphore.h>

void queue_empty(queue *q) {
    q->size = FIFO_SIZE;
    q->head = 0;
    q->tail = 0;
}

extern int queue_put(queue *q, filter_t ref){

    q->aref[q->tail] = ref;
    q->tail = (q->tail + 1) % FIFO_SIZE;

    return 0;
}

extern filter_t queue_pop(queue *q){

    filter_t val = q->aref[q->head];
    q->head = (q->head + 1) % FIFO_SIZE;

    return val;
}