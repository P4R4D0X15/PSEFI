#ifndef COMMON_H
#define COMMON_H
#include "../../common/common.h"
#endif
#include <stdint.h>

typedef struct {
    uint8_t  bfType[2];
    uint32_t bfSize;
    uint16_t bfReserved1;
    uint16_t bfReserved2;
    uint32_t bfOffBits;
} bitmapfileheader;

typedef enum {
    BI_RGB            = 0,
    BI_RLE8           = 1,
    BI_RLE4           = 2,
    BI_BITFIELDS      = 3,
    BI_JPEG           = 4,
    BI_PNG            = 5,
    BI_ALPHABITFIELDS = 6,
    BI_CMYK           = 11,
    BI_CMYKRLE8       = 12,
    BI_CMYKRLE4       = 13
} compression;

typedef struct {
    uint32_t biSize;
    int32_t biWidth;
    int32_t biHeight;
    uint16_t biPlanes;
    uint16_t biBitCount;
    compression biCompression;
    uint32_t biSizeImage;
    int32_t biXPelsPerMeter;
    int32_t biYPelsPerMeter;
    uint32_t biClrUsed;
    uint32_t biClrImportant;
} bitmapinfoheader;

typedef struct {
    uint8_t blue;
    uint8_t green;
    uint8_t red;
    uint8_t reserved;
} colors;


typedef struct {
    bitmapfileheader header;

    void *infoHeader;

    colors *palette;

    uint8_t *pixelData;

} bitmap;

typedef struct{
    uint8_t blue;
    uint8_t green;
    uint8_t red;
} pixelColors;

// read_fileheader : Lit le header de l'image associée à bmp_fd et 
//      renvoie un pointeur contenant les données du header du fichier 
//      (taille totale du fichier, types de compression, ...). Renvoie 
//      un pointeur nul en cas d'erreur de lecture ou d'allocation 
//      du buffer
extern bitmapfileheader *read_header(int bmp_fd);

// read_infoheader : Lit les infos de l'image stockées dans son header 
//      sur le fichier associé à bmp_fd et renvoie un pointeur
//      contenant les infos du header de l'image 
//      (taille de l'image, largeur et hauteur de l'image). Renvoie 
//      un pointeur nul en cas d'erreur de lecture ou d'allocation 
//      du buffer
extern bitmapinfoheader *read_infoheader(int bmp_fd);

// write_header : Écrit le header de l'image stockées dans son header 
//      sur le fichier associé à bmp_fd et renvoie 0 en cas de succès. 
//      Renvoie -1 en cas d'erreur d'écriture
extern int write_header(int bmp_fd, bitmapfileheader *hd_buf);

// write_infoheader : Écrit les infos de l'image stockées dans son header 
//      sur le fichier associé à bmp_fd et renvoie 0 en cas de succès. 
//      Renvoie -1 en cas d'erreur d'écriture
extern int write_infoheader(int bmp_fd, bitmapinfoheader *info_buf);

// read_all_pixels : Lit et copie dans un tableau tous les pixels, avec leurs valeurs RGB, de l'image associée à bmp_fd.
//      Renvoie le tableau créé en cas de succès et renvoie un pointeur nul en cas d'échec
extern pixelColors **read_all_pixels(int bmp_fd, int size);

// read_pixel : Lit un pixel de l'image associée à bmp_fd et copie les valeurs RGB dans une structure de pixel. 
//      Renvoie la structure créée en cas de succès et renvoie un pointeur nul en cas d'échec
extern pixelColors *read_pixel(int bmp_fd);

// dispose_pixel : Libère l'espace alloué à chaque pixel et au tableau de pixel
extern void dispose_pixels(pixelColors **pxs, int size);

// write_pixels : Écrit les pixels stockés dans le tableau de pixels associé au pointeur pxs dans le fichier associé à bmp_fd.
//      Renvoie -1 en cas d'erreur d'écriture et renvoie 0 sinon 
extern int write_pixels(int bmp_fd, pixelColors **pxs, int size);