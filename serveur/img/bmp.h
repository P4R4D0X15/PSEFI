#ifndef COMMON_H
#define COMMON_H
#include "../../common/common.h"
#endif
#include <stdint.h>

// File Header
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

/* --- Palette entry --- */

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