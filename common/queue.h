#ifndef COMMON_H
#define COMMON_H
#include "common.h"

#define FIFO_SIZE 10

typedef struct queue {
    //sem_t *fifo;
    //sem_t *empty;
    //sem_t *full;
    size_t size;
    size_t tail;
    filter_t aref[FIFO_SIZE];
    size_t head;
} queue;

//  queue_empty : tente d'allouer les ressources nécessaires pour gérer un
//    nouvelle file (FIFO) initialement vide. Renvoie un pointeur nul en cas de
//    dépassement de capacité. Renvoie sinon un pointeur vers le contrôleur
//    associée à la file.
extern void queue_empty(queue *q);

//  queue_dispose : sans effet si *qptr vaut un pointeur nul. Libère sinon
//    les ressources allouées la gestion d'une file associé à *qptr puis
//    affecte un pointeur nul à *qptr.
extern void queue_dispose(queue **qptr);

//  queue_put : 
//      - Si ref vaut un pointeur nul => Renvoie -1
//      - Si la file associée à q est une file pleine => Renvoie -1
//      - Sinon tente d'enfiler ref dans la file associée à q
//          - En cas de dépassement de capacité => Renvoie -1
//          - Sinon => Renvoie 0
extern int queue_put(queue *q, filter_t ref);

//  queue_pop : 
//      - Si q est associée à une file vide => Renvoie un pointeur nul
//      - Sinon tente de défiler la file associée à q et renvoie la valeur
//      qui était précédemment en tête
extern filter_t queue_pop(queue *q);

#endif
