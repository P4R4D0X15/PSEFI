#ifndef COMMON_H
#define COMMON_H
#include "../../common/common.h"
#endif

#include "bmp.h"

typedef struct {
    pixelColors **pxs;
    int start;
    int end;
    pthread_t th;
} thdata_t;

// grayscale : Applique le filtre de nuance de gris sur une partition de l'image
// correspond à la routine d'un thread
extern void *grayscale(void *arg);

// negative : Applique le filtre négatif sur une partition de l'image
// correspond à la routine d'un thread
extern void *negative(void *arg);

extern void *red_filter(void *arg);
extern void *green_filter(void *arg);
extern void *blue_filter(void *arg);