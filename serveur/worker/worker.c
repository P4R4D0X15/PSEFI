#ifndef COMMON_H
#define COMMON_H
#include "../../common/common.h"
#include <fcntl.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#endif

#include <pthread.h>
#include "../img/filter.h"
#include "worker.h"

#define NB_THREADS 8

bool validate_filter(int filter) {
    return filter == GRAYSCALE
            || filter == NEGATIVE
            || filter == RED_FILTER
            || filter == GREEN_FILTER
            || filter == BLUE_FILTER;
}

int work(filter_t req){

    // Ouverture du tube réponse en écriture seule
    char tubename[256];
    sprintf(tubename, "/tmp/fifo_rep_%d", req.pid);

    // Answer buffer
    char ans_buf[512];

    int ans_tub = open(tubename, O_WRONLY, 0666);
    if (ans_tub == -1) {
        return -1;
    }

    // Vérification du filtre
    if (!validate_filter(req.filter)) {
        sprintf(ans_buf, RED "/!\\ ***** Filtre non reconnu ***** /!\\"RESET"\n");
        write(ans_tub, ans_buf, strlen(ans_buf));
        close(ans_tub);
        exit(EXIT_FAILURE);
    }

    // Changement de répertoire de travail 
    if (chdir("../output") == -1) {
        sprintf(ans_buf, RED "/!\\ ***** Cannot access to output directory ***** /!\\"RESET"\n");
        write(ans_tub, ans_buf, strlen(ans_buf));
        close(ans_tub);
        exit(EXIT_FAILURE);
    }

    // Ouverture de l'image
    int bmp_file = open(req.path, O_RDONLY, 0666);
    if (bmp_file == -1) {
        sprintf(ans_buf, RED "/!\\ ***** Erreur sur l'ouverture de l'image ***** /!\\"RESET"\n");
        write(ans_tub, ans_buf, strlen(ans_buf));
        close(ans_tub);
        exit(EXIT_FAILURE);
    }

    // Lecture du header du fichier
    bitmapfileheader *hd_buf = read_header(bmp_file);
    if (hd_buf == nullptr) {
        sprintf(ans_buf, RED "/!\\ ***** Erreur lecture du header de l'image ***** /!\\"RESET"\n");
        write(ans_tub, ans_buf, strlen(ans_buf));
        close(ans_tub);
        close(bmp_file);
        exit(EXIT_FAILURE);
    }
    
    // Lecture des infos du header de l'image
    bitmapinfoheader *info_buf = read_infoheader(bmp_file);
    if (info_buf == nullptr) {
        sprintf(ans_buf, RED "/!\\ ***** Erreur lecture des infos du header de l'image ***** /!\\"RESET"\n");
        write(ans_tub, ans_buf, strlen(ans_buf));
        close(ans_tub);
        close(bmp_file);
        free(hd_buf);
        exit(EXIT_FAILURE);
    }

    if (info_buf->biBitCount != 24) {
        sprintf(ans_buf, RED"/!\\ ***** Erreur mauvais formats de pixels ***** /!\\"RESET"\n");
        close(bmp_file);
        write(ans_tub, ans_buf, strlen(ans_buf));
        close(ans_tub);
        free(hd_buf);
        free(info_buf);
        exit(EXIT_FAILURE);
    }

    thdata_t *thd = malloc(sizeof(*thd) * NB_THREADS);
    if (thd == nullptr) {
        sprintf(ans_buf, RED"/!\\ ***** Erreur allocation des zones de travail des threads ***** /!\\"RESET"\n");
        close(bmp_file);
        write(ans_tub, ans_buf, strlen(ans_buf));
        close(ans_tub);
        free(hd_buf);
        free(info_buf);
        exit(EXIT_FAILURE);
    }

    int nb_pixels = info_buf->biSizeImage / 3;

    // Lecture des pixels de l'image
    pixelColors **pxs = read_all_pixels(bmp_file, nb_pixels);
    if (pxs == nullptr) {
        sprintf(ans_buf, RED"/!\\ ***** Erreur Allocation des pixels de l'image ***** /!\\"RESET"\n");
        close(bmp_file);
        write(ans_tub, ans_buf, strlen(ans_buf));
        close(ans_tub);
        free(hd_buf);
        free(info_buf);
        exit(EXIT_FAILURE);
    }

    // Vérification de la compression de l'image
    if (info_buf->biCompression != BI_RGB) {
        sprintf(ans_buf, RED"/!\\ ***** Mauvais type de compression d'image ***** /!\\"RESET"\n");
        close(bmp_file);
        write(ans_tub, ans_buf, strlen(ans_buf));
        close(ans_tub);
        dispose_pixels(thd->pxs, nb_pixels);
        free(thd);
        free(hd_buf);
        free(info_buf);
        exit(EXIT_FAILURE);
    }

    // Ouverture du fichier de sortie
    char filename[256];
    sprintf(filename, "output_%d.bmp", req.pid);

    int output = open(filename, O_CREAT | O_WRONLY | O_EXCL, 0666);
    if (output == -1) {
        sprintf(ans_buf, RED"/!\\ ***** Erreur ouverture du fichier de sortie ***** /!\\"RESET"\n");
        close(bmp_file);
        write(ans_tub, ans_buf, strlen(ans_buf));
        close(ans_tub);
        dispose_pixels(thd->pxs, nb_pixels);
        free(thd);
        free(hd_buf);
        free(info_buf);
        exit(EXIT_FAILURE);
    }

    // Ecriture des headers dans l'image de sortie
    if (write_header(output, hd_buf) == -1) {
        sprintf(ans_buf, RED"/!\\ ***** Erreur écriture du header du fichier de sortie ***** /!\\"RESET"\n");
        close(bmp_file);
        close(output);
        write(ans_tub, ans_buf, strlen(ans_buf));
        close(ans_tub);
        dispose_pixels(thd->pxs, nb_pixels);
        free(thd);
        free(hd_buf);
        free(info_buf);
        exit(EXIT_FAILURE);
    }

    if (write_infoheader(output, info_buf) == -1) {
        sprintf(ans_buf, RED"/!\\ ***** Erreur écriture des infos du header du fichier de sortie ***** /!\\"RESET"\n");
        close(bmp_file);
        close(output);
        write(ans_tub, ans_buf, strlen(ans_buf));
        close(ans_tub);
        dispose_pixels(thd->pxs, nb_pixels);
        free(thd);
        free(hd_buf);
        free(info_buf);
        exit(EXIT_FAILURE);
    }

    // Traitement des pixels
    void *filter_fun = nullptr;

    switch(req.filter) {
        case GRAYSCALE:
            filter_fun = grayscale;
            break;
        case NEGATIVE:
            filter_fun = negative;
            break;
        case RED_FILTER:
            filter_fun = red_filter;
            break;
        case GREEN_FILTER:
            filter_fun = green_filter;
            break;
        case BLUE_FILTER:
            filter_fun = blue_filter;
            break;
    }

    int slice_size = nb_pixels / NB_THREADS;

    for (int i = 0; i < NB_THREADS; i++) {

        thd[i].start = i * slice_size;
        thd[i].end = (i == NB_THREADS - 1) ? nb_pixels : thd[i].start + slice_size;
        thd[i].pxs = pxs;
        
        if (pthread_create(&thd[i].th, nullptr, filter_fun, &thd[i]) != 0){
            sprintf(ans_buf, RED"/!\\ ***** Erreur création d'un threads ***** /!\\"RESET"\n");
            close(bmp_file);
            close(output);
            write(ans_tub, ans_buf, strlen(ans_buf));
            close(ans_tub);
            dispose_pixels(thd->pxs, nb_pixels);
            free(thd);
            free(hd_buf);
            free(info_buf);
            exit(EXIT_FAILURE);   
        }

    } 

    for (int i = 0; i < NB_THREADS; i++) {
        if (pthread_join(thd[i].th, NULL) != 0){
            sprintf(ans_buf, RED"/!\\ ***** Erreur lors de l'attente d'un thread ***** /!\\"RESET"\n");
            close(bmp_file);
            close(output);
            write(ans_tub, ans_buf, strlen(ans_buf));
            close(ans_tub);
            dispose_pixels(thd->pxs, nb_pixels);
            free(thd);
            free(hd_buf);
            free(info_buf);
            exit(EXIT_FAILURE);
        }
    }

    if (write_pixels(output, thd->pxs, nb_pixels) == -1) {
        sprintf(ans_buf, RED"/!\\ ***** Erreur lors de l'écriture des pixels ***** /!\\"RESET"\n");
        close(bmp_file);
        close(output);
        write(ans_tub, ans_buf, strlen(ans_buf));
        close(ans_tub);
        dispose_pixels(thd->pxs, nb_pixels);
        free(thd);
        free(hd_buf);
        free(info_buf);
        exit(EXIT_FAILURE);
    }

    sprintf(ans_buf, "Votre image se trouve dans le répertoire /PSEFI/output/%s", filename);
    write(ans_tub, ans_buf, strlen(ans_buf));

    close(bmp_file);
    close(output);
    close(ans_tub);
    dispose_pixels(thd->pxs, nb_pixels);
    free(thd);
    free(hd_buf);
    free(info_buf);
    return EXIT_SUCCESS;
}