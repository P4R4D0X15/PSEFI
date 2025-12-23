#include "filter.h"

void *grayscale (void *arg) {
    thdata_t *thd = arg; 
    
    for (int i = thd->start; i < thd->end; i++) {
        uint8_t gray = 0.299 * thd->pxs[i]->red + 0.587 * thd->pxs[i]->green + 0.114 * thd->pxs[i]->blue;

        thd->pxs[i]->red   = gray;
        thd->pxs[i]->green = gray;
        thd->pxs[i]->blue  = gray;
    }
    return nullptr;
}

void *negative (void *arg) {
    thdata_t *thd = arg;
    for (int i = thd->start; i < thd->end; i++) {
        thd->pxs[i]->blue = 255 - thd->pxs[i]->blue;
        thd->pxs[i]->green = 255 - thd->pxs[i]->green;
        thd->pxs[i]->red = 255 - thd->pxs[i]->red;
    }
    return nullptr;
}

void *red_filter(void *arg) {
    thdata_t *thd = arg;
    for (int i = thd->start; i < thd->end; i++) {
        thd->pxs[i]->blue = 0;
        thd->pxs[i]->green = 0;
    }
    return nullptr;
}


void *green_filter(void *arg) {
    thdata_t *thd = arg;
    for (int i = thd->start; i < thd->end; i++) {
        thd->pxs[i]->blue = 0;
        thd->pxs[i]->red = 0;
    }
    return nullptr;
}


void *blue_filter(void *arg) {
    thdata_t *thd = arg;
    for (int i = thd->start; i < thd->end; i++) {
        thd->pxs[i]->red = 0;
        thd->pxs[i]->green = 0;
    }
    return nullptr;
}