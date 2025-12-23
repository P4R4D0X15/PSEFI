#include "common.h"

// Maybe Rework pour renvoyer un pointeur de filter_t vu qu'on a 
// plus le problème de la mémoire partagée

filter_t cpy_ref(filter_t ref){
    filter_t cpy;
    
    cpy.filter = ref.filter;
    cpy.pid = ref.pid;
    
    strcpy(cpy.path, ref.path);

    for (int i = 0; i < sizeof(ref.parameters)/sizeof(ref.parameters[0]); i++) {
        cpy.parameters[i] = ref.parameters[i];
    }
    
    return cpy;
}