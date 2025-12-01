#include "queue.h"
#include "../error/error.h"
#include <semaphore.h>
#include <stdlib.h>
#include <string.h>

void queue_empty(queue *q) {
    q->size = FIFO_SIZE;
    q->head = 0;
    q->tail = 0;

    filter_t empty_req;

    empty_req.pid = 0;
    strcpy(empty_req.path, "");
    empty_req.filter = 0;
    for (int i = 0; i < 5; i++) {
        empty_req.parameters[i] = -1;
    }
}

extern void queue_dispose(queue **qptr){
    if (*qptr == nullptr) {
        return;
    }
    free(*qptr);
    *qptr = nullptr;
}

extern int queue_put(queue *q, filter_t ref){

    q->aref[q->tail] = ref;
    q->tail = (q->tail + 1) % FIFO_SIZE;

    return 0;
}

extern filter_t queue_pop(queue *q){

    printf("%zu\n", q->head);

    filter_t val = q->aref[q->head];
    q->head = (q->head + 1) % FIFO_SIZE;

    return val;
}