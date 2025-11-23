#include "common.h"

void *cpy_ref(const filter_t *ref, holdall *ha){
    filter_t *cpy = malloc_safe(sizeof(*ref), 
                                  "Erreur Allocation de la copie de la référence\n",
                                  0);
    cpy->filter = ref->filter;
    for (int i = 0; i < 256; i++) {
        cpy->path[i] = ref->path[i];
    }
    for (int i = 0; i < 5; i++) {
        cpy->parameters[i] = ref->parameters[i];
    }
    cpy->pid = ref->pid;
    holdall_put(ha, cpy);
    return cpy;
}