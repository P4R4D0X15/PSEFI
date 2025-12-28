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
//              (<=> S'il est gérer par le serveur)
extern bool validate_filter(int filter);