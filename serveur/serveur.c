#include "../error/error.h"
#include "../common/queue.h"
//#include "../holdall/holdall.h"
#include <fcntl.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>

#ifndef COMMON_H
#define COMMON_H
#include "../common/common.h"
#endif

#define NB_THREAD 8

typedef struct {
    bool verbose;
}opt_t;

int main (int argc, char **argv) {
    
    opt_t *op = malloc(sizeof(*op));
    if (op == nullptr) {
        fprintf(stderr, RED "/!\\ ====== Erreur d'allocation de la structure des options ====== /!\\" RESET);
        exit(EXIT_FAILURE);
    }

    op->verbose = false;

    int opt;

    while ((opt = getopt(argc, argv, "vh")) != -1) {
        switch (opt) {
        case 'v':
            op->verbose = true;
            break;
        case 'h':
            help_serv();
            break;
        default:
            fprintf(stderr, RED "/!\\ ====== Option non reconnue ====== /!\\ " RESET "\n");
            help_serv();
        }
    }

    /* holdall *ha = holdall_empty();
    if (ha == nullptr) {
        free(op);
        exit(EXIT_FAILURE);
    } */

    shm_unlink("/seg");

    // Création du segment de mémoire partagée 
    int seg = shm_open("/seg", O_CREAT | O_RDWR | O_EXCL, 0666);
    if (seg == -1) {
        //holdall_dispose(&ha);
        free(op);
        perror("shm_open");
        exit(EXIT_FAILURE);
    }

    // Redimensionnement du segment de mémoire partagée
    if (ftruncate(seg, sizeof(queue)) == -1) {
        //holdall_dispose(&ha);
        free(op);
        shm_unlink("/seg");
        perror("ftruncate");
        exit(EXIT_FAILURE);
    }

    // Mappage de la file en mémoire
    volatile queue *q = mmap(nullptr, sizeof(*q), PROT_READ | PROT_WRITE, MAP_SHARED, seg, 0);
    if (q == MAP_FAILED) {
        //holdall_dispose(&ha);
        free(op);
        shm_unlink("/seg");
        perror("mmap");
        exit(EXIT_FAILURE);
    }

    queue_empty((queue *) q);

    sem_unlink("/fifo");
    sem_unlink("/empty");
    sem_unlink("/full");

    // Semaphore liée à la file pour éviter l'écriture simultanée
    sem_t *fifo = sem_open("/fifo", O_CREAT | O_RDWR, 0666, 1);
    if (fifo == SEM_FAILED) {
        shm_unlink("/seg");
        //holdall_dispose(&ha);
        munmap((void *) q, FIFO_SIZE * sizeof(*q));
        free(op);   
        perror("sem_open");
        exit(EXIT_FAILURE);
    }
    // Sémaphore liée à la file pour vérifier si la file est vide
    sem_t *empty = sem_open("/empty", O_CREAT | O_RDWR, 0666, FIFO_SIZE);
    if (empty == SEM_FAILED) {
        sem_unlink("/fifo");
        sem_close(fifo);
        shm_unlink("/seg");
        //holdall_dispose(&ha);
        munmap((void *) q, sizeof(*q));
        free(op);
        perror("sem_open");
        exit(EXIT_FAILURE);
    }
    // Sémaphore liée à la file pour vérifier si la file est pleine
    sem_t *full = sem_open("/full", O_CREAT | O_RDWR, 0666, 0);
    if (full == SEM_FAILED) {
        sem_unlink("/fifo");
        sem_close(fifo);
        sem_unlink("/empty");
        sem_close(empty);
        shm_unlink("/seg");
        //holdall_dispose(&ha);
        munmap((void *) q,sizeof(*q));
        free(op);
        perror("sem_open");
        exit(EXIT_FAILURE);
    }

    // Traitement des requètes
    while (1) {
        CHECK_RETURN(sem_wait(full), -1, "sem_wait", true);
        CHECK_RETURN(sem_wait(fifo), -1, "sem_wait", true);
        
        filter_t cli_req = queue_pop((queue *) q);

        CHECK_RETURN(sem_post(fifo), -1, "sem_post", true);
        CHECK_RETURN(sem_post(empty), -1, "sem_post", true);
        
        printf("Le Filtre voulu :%d\n", cli_req.filter);
        printf("Voici le path que le pere a reçu: %s\n", cli_req.path);
    }
}