#include "common.h"

void *cpy_ref(const filter_t *ref, holdall *ha){
    filter_t *cpy = malloc_safe(sizeof(*ref), 
                                  "Erreur Allocation de la copie de la référence\n",
                                  0);
   
    printf("%d\t%s\n", cpy->pid, cpy->path);
    for (int i = 0; i < 5; i++) {
        printf("%d\t", cpy->parameters[i]);
    }
    printf("\n");
    cpy->filter = ref->filter;
    cpy->pid = ref->pid;
    
    
    strcpy(cpy->path, ref->path);

    for (int i = 0; i < sizeof(ref->parameters)/sizeof(ref->parameters[0]); i++) {
        cpy->parameters[i] = ref->parameters[i];
    }
    
    printf("C'est le holdall évidemment je pense que tu sais juste pas coder\n");
    holdall_put(ha, cpy);
    free((void *) ref);
    return cpy;
}