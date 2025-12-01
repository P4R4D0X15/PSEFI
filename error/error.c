#include "error.h"
#include <semaphore.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>

void help_serv() {
    fprintf(stderr, "Utilisation ./serveur [OPT]\n");
    fprintf(stderr, "OPT : \n");
    fprintf(stderr, "\t -h : Affiche ce message d'aide \n");
    fprintf(stderr, "\t -v : Active le mode verbeux \n");
    exit(EXIT_FAILURE);
}

void help_cli() {
    fprintf(stderr, "Utilisation ./client <chemin de l'image> <ID du filtre>\n");
    fprintf(stderr, "ID du filtre : \n");
    fprintf(stderr, "\t 1 : filtre gris \n");
    exit(EXIT_FAILURE);
}

void *malloc_safe(int size, const char *err_msg, int nbptr, ...) {
    void *val = malloc(size);
    va_list(ap);
    if (val == nullptr) {
        va_start(ap, nbptr);
        for (int i = 1; i <= nbptr; i++) {
            free(va_arg(ap, void *));
        }
        va_end(ap);
        fprintf(stderr, RED "***** %s *****" RESET "\n", err_msg);
        exit(EXIT_FAILURE);
    }
    return val;
}