#include "bmp.h"

bitmapfileheader *read_header(int bmp_fd) {
    bitmapfileheader *hd_buf = malloc(sizeof(*hd_buf));
    if (hd_buf == nullptr) {
        return nullptr;
    }

    if (read(bmp_fd, hd_buf->bfType, sizeof(hd_buf->bfType)) == -1
        || read(bmp_fd, &(hd_buf->bfSize), sizeof(hd_buf->bfSize)) == -1
        || read(bmp_fd, &(hd_buf->bfReserved1), sizeof(hd_buf->bfReserved1)) == -1
        || read(bmp_fd, &(hd_buf->bfReserved2), sizeof(hd_buf->bfReserved2))== -1
        || read(bmp_fd, &(hd_buf->bfOffBits), sizeof(hd_buf->bfOffBits)) == -1
    ) {
        
        free(hd_buf);
        return nullptr;
    }

    return hd_buf;
}

bitmapinfoheader *read_infoheader(int bmp_fd){
    bitmapinfoheader *info_buf = malloc(sizeof(*info_buf));
    if (info_buf == nullptr) {
        return nullptr;
    }
    if (read(bmp_fd, &(info_buf->biSize), sizeof(info_buf->biSize)) == -1
        || read(bmp_fd, &(info_buf->biWidth), sizeof(info_buf->biWidth)) == -1
        || read(bmp_fd, &(info_buf->biHeight), sizeof(info_buf->biHeight)) == -1
        || read(bmp_fd, &(info_buf->biPlanes), sizeof(info_buf->biPlanes)) == -1
        || read(bmp_fd, &(info_buf->biBitCount), sizeof(info_buf->biBitCount)) == -1
        || read(bmp_fd, &(info_buf->biCompression), sizeof(info_buf->biCompression)) == -1
        || read(bmp_fd, &(info_buf->biSizeImage), sizeof(info_buf->biSizeImage)) == -1
        || read(bmp_fd, &(info_buf->biXPelsPerMeter), sizeof(info_buf->biXPelsPerMeter)) == -1
        || read(bmp_fd, &(info_buf->biYPelsPerMeter), sizeof(info_buf->biYPelsPerMeter)) == -1
        || read(bmp_fd, &(info_buf->biClrUsed), sizeof(info_buf->biClrUsed)) == -1
        || read(bmp_fd, &(info_buf->biClrImportant), sizeof(info_buf->biClrImportant)) == -1
    ) {
    
        free(info_buf);
        return nullptr;
    }
    return info_buf;
}

int write_header(int bmp_fd, bitmapfileheader *hd_buf){
    if (write(bmp_fd, hd_buf->bfType, sizeof(hd_buf->bfType)) == -1
        || write(bmp_fd, &(hd_buf->bfSize), sizeof(hd_buf->bfSize)) == -1
        || write(bmp_fd, &(hd_buf->bfReserved1), sizeof(hd_buf->bfReserved1)) == -1
        || write(bmp_fd, &(hd_buf->bfReserved2), sizeof(hd_buf->bfReserved2)) == -1
        || write(bmp_fd, &(hd_buf->bfOffBits), sizeof(hd_buf->bfOffBits)) == -1
    ) {
        return -1;
    }
    return 0;
}

int write_infoheader(int bmp_fd, bitmapinfoheader *info_buf){
    if (write(bmp_fd, &(info_buf->biSize), sizeof(info_buf->biSize)) == -1
        || write(bmp_fd, &(info_buf->biWidth), sizeof(info_buf->biWidth)) == -1
        || write(bmp_fd, &(info_buf->biHeight), sizeof(info_buf->biHeight)) == -1
        || write(bmp_fd, &(info_buf->biPlanes), sizeof(info_buf->biPlanes)) == -1
        || write(bmp_fd, &(info_buf->biBitCount), sizeof(info_buf->biBitCount)) == -1
        || write(bmp_fd, &(info_buf->biCompression), sizeof(info_buf->biCompression)) == -1
        || write(bmp_fd, &(info_buf->biSizeImage), sizeof(info_buf->biSizeImage)) == -1
        || write(bmp_fd, &(info_buf->biXPelsPerMeter), sizeof(info_buf->biXPelsPerMeter)) == -1
        || write(bmp_fd, &(info_buf->biYPelsPerMeter), sizeof(info_buf->biYPelsPerMeter)) == -1
        || write(bmp_fd, &(info_buf->biClrUsed), sizeof(info_buf->biClrUsed)) == -1
        || write(bmp_fd, &(info_buf->biClrImportant), sizeof(info_buf->biClrImportant)) == -1
    ) {    
        return -1;
    }
    return 0;
}

pixelColors **read_all_pixels(int bmp_fd, int size){

    pixelColors **pxs = malloc(sizeof(*pxs) * size);
    if (pxs == nullptr){
        return nullptr;
    }

    int j = 0;
    for(int i = 0; i < size; i++){
        pxs[j] = read_pixel(bmp_fd);
        if (pxs[j] == nullptr) {
            for (int k = 0; k < i; k++) {
                free(pxs[k]);
            }
            free(pxs);
            return nullptr;
        }
        j++; 
    }
    
    return pxs;
}

pixelColors *read_pixel(int bmp_fd){
    pixelColors *px = malloc(sizeof(*px));
    if (px == nullptr) {
        return nullptr;
    }
    if (read(bmp_fd, &(px->blue), sizeof(px->blue)) == -1
        || read(bmp_fd, &(px->green), sizeof(px->green)) == -1
        || read(bmp_fd, &(px->red), sizeof(px->red)) == -1) {
        free(px);
        return nullptr;
    }
    return px;
}

void dispose_pixels(pixelColors **pxs, int size) {
    for (int i = 0; i < size; i++) {
        free(pxs[i]);
    }
    free(pxs);
}

int write_pixels(int bmp_fd, pixelColors **pxs, int size){
    for (int i = 0; i < size; i++) {
        if (write(bmp_fd, &(pxs[i]->blue), sizeof(pxs[i]->blue)) == -1
            || write(bmp_fd, &(pxs[i]->green), sizeof(pxs[i]->green)) == -1
            || write(bmp_fd, &(pxs[i]->red), sizeof(pxs[i]->red)) == -1) {
            return -1;
        }
    }
    return 0;
}