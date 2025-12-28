#include "error.h"
#include <semaphore.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <unistd.h>

void help_serv() {
    fprintf(stderr, "Utilisation ./serveur [OPT]\n");
    fprintf(stderr, "OPT : \n");
    fprintf(stderr, "\t -h : Affiche ce message d'aide \n");
    exit(EXIT_FAILURE);
}

void help_cli() {
    fprintf(stderr, "Utilisation ./client <chemin de l'image> <ID du filtre>\n");
    fprintf(stderr, "ID du filtre : \n");
    fprintf(stderr, "\t 1 : filtre gris \n");
    fprintf(stderr, "\t 2 : filtre négatif \n");
    fprintf(stderr, "\t 3 : filtre rouge \n");
    fprintf(stderr, "\t 4 : filtre vert \n");
    fprintf(stderr, "\t 5 : filtre bleu \n");
    exit(EXIT_FAILURE);
}

// sighandler : Supprime les sémaphores et le segment de mémoire 
// partagée lors de la réception du signal sig
void sighandler(int sig) {
    shm_unlink("/seg");
    sem_unlink("/fifo");
    sem_unlink("/full");
    sem_unlink("/empty");
    _exit(EXIT_FAILURE);
}