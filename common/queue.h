#ifndef COMMON_H
#define COMMON_H
#include "common.h"
#include <stddef.h>

typedef struct queue queue;

//  queue_empty : tente d'allouer les ressources nécessaires pour gérer un
//    nouvelle file (FIFO) initialement vide. Renvoie un pointeur nul en cas de
//    dépassement de capacité. Renvoie sinon un pointeur vers le contrôleur
//    associée à la file.
extern queue *queue_empty();

//  queue_dispose : sans effet si *qptr vaut un pointeur nul. Libère sinon
//    les ressources allouées la gestion d'une file associé à *qptr puis
//    affecte un pointeur nul à *qptr.
extern void queue_dispose(queue **qptr);

//  queue_put : 
//      - Si ref vaut un pointeur nul => Renvoie un pointeur nul
//      - Si la file associée à q est une file pleine => Renvoie un pointeur nul
//      - Sinon tente d'enfiler ref dans la file associée à q
//          - En cas de dépassement de capacité => Renvoie un pointeur nul
//          - Sinon => Renvoie ref
extern void *queue_put(queue *q, void *ref);

//  queue_pop : 
//      - Si q est associée à une file vide => Renvoie un pointeur nul
//      - Sinon tente de défiler la file associée à q et renvoie la valeur
//      qui était précédemment en tête
extern void *queue_pop(queue *q);

#endif
