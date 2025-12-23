#include "../error/error.h"
#include "../common/queue.h"
#include <fcntl.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <unistd.h>

#ifndef COMMON_H
#define COMMON_H
#include "../common/common.h"
#endif

#define BUFFER_SIZE 4096

typedef struct{
    int filter;
    const char *path;
}opt_t;

int main (int argc, char **argv) {

    if (argc < 3) {
        help_cli();
        exit(EXIT_FAILURE);
    }

    opt_t *op = malloc(sizeof(*op));
    if (op == nullptr) {
        exit(EXIT_FAILURE);
    }

    int opt;

    while ((opt = getopt(argc, argv, "hp:f:")) != -1) {
        switch (opt) {
        case 'h':
            help_cli();
            break;
        case 'p':
            op->path = optarg;
            break;
        case 'f':
            op->filter = atoi(optarg);
            break;
        default:
            free(op);
            fprintf(stderr, RED "/!\\ ====== Option non reconnue ====== /!\\ " RESET "\n");
            help_cli();
        }
    }

    pid_t pid = getpid();    

    int seg = shm_open("/seg", O_RDWR, 0666);
    if (seg == -1) {
        free(op);
        perror("shm_open");
        exit(EXIT_FAILURE);
    }

    volatile queue *q = mmap(nullptr, sizeof(*q), PROT_READ | PROT_WRITE, MAP_SHARED, seg, 0); 
    if (q == MAP_FAILED) {
        shm_unlink("/seg");
        free(op);
        perror("mmap");
        exit(EXIT_FAILURE);
    }

    // Semaphore liée à la file pour éviter l'écriture simultanée
    sem_t *fifo = sem_open("/fifo", O_RDWR);
    if (fifo == SEM_FAILED) {
        munmap((void *) q, FIFO_SIZE * sizeof(*q));
        free(op);
        perror("sem_open");
        exit(EXIT_FAILURE);
    }
    // Sémaphore liée à la file pour vérifier si la file est vide
    sem_t *empty = sem_open("/empty", O_RDWR);
    if (empty == SEM_FAILED) {
        sem_unlink("/fifo");
        sem_close(fifo);
        munmap((void *) q, sizeof(*q));
        free(op);
        perror("sem_open");
        exit(EXIT_FAILURE);
    }

    // Sémaphore liée à la file pour vérifier si la file est pleine
    sem_t *full = sem_open("/full", O_RDWR);
    if (full == SEM_FAILED) {
        sem_unlink("/empty");
        sem_close(empty);
        sem_unlink("/fifo");
        sem_close(fifo);
        munmap((void *) q,sizeof(*q));
        free(op);
        perror("sem_open");
        exit(EXIT_FAILURE);
    }

    filter_t req;

    req.pid = pid;
    strcpy(req.path, op->path);
    req.filter = op->filter;
    for (int i = 0; i < 5; i++) {
        req.parameters[i] = -1;
    }

    CHECK_RETURN(sem_wait(empty), -1, "sem_wait", true);
    CHECK_RETURN(sem_wait(fifo), -1, "sem_wait", true);

    queue_put((queue *) q, req);

    CHECK_RETURN(sem_post(fifo), -1, "sem_post", true);
    CHECK_RETURN(sem_post(full), -1, "sem_post", true);

    char tubename[256];
    sprintf(tubename, "/tmp/fifo_rep_%d", pid);

    char *answer_buffer = malloc(BUFFER_SIZE);
    if (answer_buffer == nullptr){
        sem_unlink("/empty");
        sem_close(empty);
        sem_unlink("/full");
        sem_close(full);
        sem_unlink("/fifo");
        sem_close(fifo);
        munmap((void *) q,sizeof(*q));
        free(op);
        perror("sem_open");
        exit(EXIT_FAILURE);
    }

    mkfifo(tubename, 0666);
    int ans = open(tubename, O_RDONLY | O_CREAT /* | O_EXCL */, 0666);
    if (ans == -1) {
        sem_unlink("/empty");
        sem_close(empty);
        sem_unlink("/full");
        sem_close(full);
        sem_unlink("/fifo");
        sem_close(fifo);
        munmap((void *) q,sizeof(*q));
        free(op);
        free(answer_buffer);
        perror("sem_open");
        exit(EXIT_FAILURE);
    }
    if (read(ans, answer_buffer,BUFFER_SIZE) == -1) {

        sem_unlink("/empty");
        sem_close(empty);
        sem_unlink("/full");
        sem_close(full);
        sem_unlink("/fifo");
        sem_close(fifo);
        munmap((void *) q,sizeof(*q));
        close(ans);
        free(op);
        free(answer_buffer);
        perror("sem_open");
        exit(EXIT_FAILURE);    
    }

    printf("%s\n", answer_buffer);
    close(ans);
    return EXIT_SUCCESS;
}