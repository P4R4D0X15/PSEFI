#include "../error/error.h"
#include "../common/queue.h"
#include "img/bmp.h"
#include "worker/worker.h"
#include <signal.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

#ifndef COMMON_H
#define COMMON_H
#include "../common/common.h"
#endif

int main (int argc, char **argv) {

    if (argc > 2) {
        fprintf(stderr, RED "/!\\ ====== Trop d'arguments sur la ligne de commande ====== /!\\" RESET "\n");
        help_serv();
        exit(EXIT_FAILURE);
    }

    // Gestion des options
    if (argc == 2) {
        if (strcmp(argv[1], "-h") != 0 ){
            fprintf(stderr, YELLOW "******** Option non reconnue ********" RESET "\n");
        }
        help_serv();
        exit(EXIT_FAILURE);
    }
        


    // Détachement du terminal
    switch (fork()) {
        case -1:
            perror("fork");
            exit(EXIT_FAILURE);
        case 0:
            if (setsid() == -1) {
                fprintf(stderr, RED "/!\\ ====== Échec au détachement du serveur au terminal ====== /!\\" RESET "\n");
                perror("setsid");
                exit(EXIT_FAILURE);
            }

            // Initialisation de la structure des signaux à traiter
            struct sigaction config;
            config.sa_handler = sighandler;
            sigemptyset(&config.sa_mask);
            config.sa_flags = 0;

            // Créatoin du gestionnaire des signaux
            if (sigaction(SIGINT, &config, nullptr) == -1) {
                perror("sigaction");
                exit(EXIT_FAILURE);
            }

            if (sigaction(SIGTERM, &config, nullptr) == -1) {
                perror("sigaction");
                exit(EXIT_FAILURE);
            }

            system_logs *slogs = empty_logs();

            shm_unlink("/seg");

            // Création du segment de mémoire partagée 
            int seg = shm_open("/seg", O_CREAT | O_RDWR | O_EXCL, 0666);
            if (seg == -1) {
                clear_logs(&slogs);
                perror("shm_open");
                exit(EXIT_FAILURE);
            }

            // Redimensionnement du segment de mémoire partagée
            if (ftruncate(seg, sizeof(queue)) == -1) {
                shm_unlink("/seg");
                clear_logs(&slogs);
                perror("ftruncate");
                exit(EXIT_FAILURE);
            }

            // Mappage de la file en mémoire
            volatile queue *q = mmap(nullptr, sizeof(*q), PROT_READ | PROT_WRITE, MAP_SHARED, seg, 0);
            if (q == MAP_FAILED) {
                shm_unlink("/seg");
                clear_logs(&slogs);
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
                munmap((void *) q, FIFO_SIZE * sizeof(*q));
                clear_logs(&slogs);
                perror("sem_open");
                exit(EXIT_FAILURE);
            }
            // Sémaphore liée à la file pour vérifier si la file est vide
            sem_t *empty = sem_open("/empty", O_CREAT | O_RDWR, 0666, FIFO_SIZE);
            if (empty == SEM_FAILED) {
                sem_unlink("/fifo");
                sem_close(fifo);
                shm_unlink("/seg");
                munmap((void *) q, sizeof(*q));
                clear_logs(&slogs);
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
                munmap((void *) q,sizeof(*q));
                clear_logs(&slogs);
                perror("sem_open");
                exit(EXIT_FAILURE);
            }

            // Traitement des requètes
            while (1) {
                CHECK_RETURN(sem_wait(full), -1, "sem_wait");
                CHECK_RETURN(sem_wait(fifo), -1, "sem_wait");
                
                filter_t cli_req = queue_pop((queue *) q);

                CHECK_RETURN(sem_post(fifo), -1, "sem_post");
                CHECK_RETURN(sem_post(empty), -1, "sem_post");

                switch (fork()) {
                    case -1:
                        sem_unlink("/fifo");
                        sem_close(fifo);
                        sem_unlink("/full");
                        sem_close(full);
                        sem_unlink("/empty");
                        sem_close(empty);
                        shm_unlink("/seg");
                        munmap((void *) q,sizeof(*q));
                        clear_logs(&slogs);
                        perror("fork");
                        exit(EXIT_FAILURE);
                    case 0:
                        filter_t cpy_req = cpy_ref(cli_req);
                        // Code du worker
                        work(cpy_req, slogs);
                        break;
                    
                    default:
                        clear_logs(&slogs);
                        break;
                }
            }

            while (waitpid(-1, nullptr, 0) > 0) {
            }
            sem_unlink("/fifo");
            sem_close(fifo);
            sem_unlink("/full");
            sem_close(full);
            sem_unlink("/empty");
            sem_close(empty);
            shm_unlink("/seg");
            munmap((void *) q,sizeof(*q));

            return EXIT_SUCCESS;
        default:
            if (wait(nullptr) == -1) {
                perror("wait");
                exit(EXIT_FAILURE);
            }
            break;
    }
    exit(EXIT_FAILURE);
}