#ifndef COMMON_H
#define COMMON_H
#include "../common/common.h"
#endif

#define PATH_SIZE 256
#define CAP_MIN 512
#define CAP_MUL 2

typedef struct {
    char origin[PATH_SIZE];
    int filter;
    char filtered_img[PATH_SIZE];
}log;

// Structure pour la gestion des requètes déjà traitées.
typedef struct {
    log **alogs;
    size_t count;
    size_t length;
} system_logs;

// empty_logs : tente d'allouer les ressources nécessaires pour gérer un
//    nouveau gestionnaire de logs initialement vide. Renvoie un pointeur nul en cas d'échec.
//    Renvoie sinon un pointeur vers ce gestionnaire.
extern system_logs *empty_logs(); 

// search : Recherche dans slogs si il existe une requète dont l'image d'origine est identique 
//      à celle associé à path et dont le filtre appliquer est égal à filter. Renvoie un pointeur 
//      nul si aucune requète n'a été trouvée. Sinon renvoie le chemin de l'image traitée
extern char *search(const char *path, int filter, system_logs *slogs);

// append_log : Ajoute une requète traités dans le gestionnaire associé à slogs.
//      Renvoie un pointeur nul en cas d'échec et la requète ajoutés en cas de succès
extern void *append_log(const char *origin_path, int filter, const char *filtered_path, system_logs *slogs);

// clear_logs : Sans effet si *slogs correspont à un pointeur nul.
//      Libère sinon l'espace alloué pour la gestion de logs, supprime 
//      toutes les images du dossier output
extern void clear_logs(system_logs **slogs);