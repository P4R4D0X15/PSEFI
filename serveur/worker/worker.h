#ifndef COMMON_H
#define COMMON_H

#include "../../common/common.h"
#include "../img/bmp.h"

#endif

// TRAVAIL DU WORKER => TRAITER L'IMAGE ASSOCIÉE AU PATH
//      1 - Test de la validité du filtre demandé
//      2 - Test que le path est valide
//          2.1 - Si le fichier existe
//          2.2 - Si le path est pas juste une chaine de 
//                  caractère de merde
//      3 - Mapper l'image en mémoire
//      4 - Se séparer en threads (8 devrait suffir)
//      5 - Attendre que les threads terminent leurs tâches
//      6 - "Construire" la réponse
//      7 - Envoyer la réponse au client
extern int work(filter_t req);


// validate_filter : Teste si le filtre est valide 
//              (<=> Si il est gérer par le serveur)
extern bool validate_filter(int filter);

// read_fileheader : Lit le header de l'image associée à bmp_fd et 
//      renvoie un pointeur contenant les données du header du fichier 
//      (taille totale du fichier, types de compression, ...). Renvoie 
//      un pointeur nul en cas d'erreur de lecture ou d'allocation 
//      du buffer
extern bitmapfileheader *read_header(int bmp_fd);

// read_infoheader : Lit les infos de l'images stockées dans son header 
//      sur le fichier associé à bmp_fd et renvoie un pointeur
//      contenant les infos du header de l'image 
//      (taille de l'image, largeur et hauteur de l'image). Renvoie 
//      un pointeur nul en cas d'erreur de lecture ou d'allocation 
//      du buffer
extern bitmapinfoheader *read_infoheader(int bmp_fd);

// write_header : Écrit le header de l'images stockées dans son header 
//      sur le fichier associé à bmp_fd et renvoie 0 en cas de succès. 
//      Renvoie -1 en cas d'erreur d'écriture
int write_header(int bmp_fd, bitmapfileheader *hd_buf);

// write_infoheader : Écrit les infos de l'images stockées dans son header 
//      sur le fichier associé à bmp_fd et renvoie 0 en cas de succès. 
//      Renvoie -1 en cas d'erreur d'écriture
int write_infoheader(int bmp_fd, bitmapinfoheader *info_buf);

pixelColors **read_all_pixels(int bmp_fd, int size);

pixelColors *read_pixel(int bmp_fd);

void *dispose_pixels(pixelColors **pxs, int size);

int write_pixels(int bmp_fd, pixelColors **pxs, int size);