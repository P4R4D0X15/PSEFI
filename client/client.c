#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/stat.h>

#define BUFFER_SIZE 256

#define GRAYSCALE 1

typedef struct {
    pid_t pid;
    char path[256];
    int filter;
    int parameters[5];
}filter_t;

void help();

int main (int argc, char **argv) {
    if (argc < 3) {
        help();
        exit(EXIT_FAILURE);
    }

    pid_t pid = getpid();

    char *pathname = malloc(BUFFER_SIZE);
    if (pathname == nullptr) {
        fprintf(stderr, "pathname : error malloc");
        exit(EXIT_FAILURE);
    }
    pathname = argv[1];

    int filter = atoi(argv[2]);
    if (filter != GRAYSCALE) {
        fprintf(stderr, "Unsupported filter\n");
        exit(EXIT_FAILURE);
    }

    char *tubename = malloc(BUFFER_SIZE);
    if (tubename == nullptr) {
        fprintf(stderr, "Error on malloc\n");
        exit(EXIT_FAILURE);
    }

    sprintf(tubename, "/tmp/fifo_rep_%d", pid);
    if (mkfifo(tubename, 0644) == -1) {
        perror("mkfifo");
        exit(EXIT_FAILURE);
    }


    
}

void help(){
    
}