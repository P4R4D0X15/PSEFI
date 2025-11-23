#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <semaphore.h>
#include <sys/mman.h>
#include <stdbool.h>
#include <getopt.h>
#include <string.h>
#include "../holdall/holdall.h"
#include "../error/error.h"

typedef struct {
    pid_t pid;
    char path[256];
    int filter;
    int parameters[5];
}filter_t;

#define GRAYSCALE 1
#define NEGATIVE 2

// cpy_ref:
//      - Si ref vaut un pointeur nul => renvoie un pointeur nul
//      - Sinon tente de créer une copie de ref :
//          - Renvoie un pointeur nul en cas de dépassement de capacité
//          - Renvoie la copie et la rajoute dans le fourre-tout sinon
extern void *cpy_ref(const filter_t *ref, holdall *ha);