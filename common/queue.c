#include "queue.h"
#include "../error/error.h"
#include <stddef.h>
#include <stdlib.h>

#define FIFO_SIZE 10

typedef struct queue {
    size_t size;
    const void **aref;
    size_t empty_cell_nb;
    size_t occupied_space;
} queue;


queue *queue_empty() {
    queue *q = malloc_safe(sizeof(*q), 
                           "Erreur Allocation de la structure de la file\n", 
                           0);
    q->size = FIFO_SIZE;
    
    q->aref = malloc_safe(FIFO_SIZE * sizeof(*(q->aref)), 
                          "Erreur Allocation de la file",
                          1,
                          q);

    q->empty_cell_nb = FIFO_SIZE;
    q->occupied_space = 0;
    return q;
}

extern void queue_dispose(queue **qptr){
    if (*qptr == nullptr) {
        return;
    }
    free((*qptr)->aref);
    free(*qptr);
    *qptr = nullptr;
}

extern void *queue_put(queue *q, void *ref){
    if (ref == nullptr) {
        return nullptr
    }
    if (q->empty_cell_nb == 0) {
        return nullptr;
    }
    for (int i = q->occupied_space ; i >= 1; i--) {
        const void *tmp = q->aref[i];
        q->aref[i] = q->aref[i-1];
        q->aref[i-1] = tmp;
    }
    q->aref[0] = ref;
    q->empty_cell_nb--;
    q->occupied_space++;
    return ref;
}

extern void *queue_pop(queue *q){
    if (q->size == 0) {
        return nullptr;
    }
    const void *val = q->aref[q->occupied_space-1];
    q->empty_cell_nb++;
    q->occupied_space--;
    return (void *) val;
}