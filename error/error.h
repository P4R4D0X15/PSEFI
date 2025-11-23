#define RED     "\x1b[31m"
#define GREEN   "\x1b[32m"
#define YELLOW  "\x1b[33m"
#define BLUE    "\x1b[34m"
#define MAGENTA "\x1b[35m"
#define CYAN    "\x1b[36m"
#define RESET   "\x1b[0m"
#define FLASH   "\x1b[5m"

#define CHECK_RETURN(arg, err_val, name_function, verbose)                                              \
    {                                                                                                   \
        if (verbose) {                                                                                  \
            printf(YELLOW "===== Test %s ======" RESET "\n", name_function);                            \
        }                                                                                               \
        if (arg == err_val) {                                                                           \
            if (verbose) {                                                                              \
                printf(RED "/!\\ ***** ERREUR DETECTE ***** /!\\" RESET "\n");                          \
            }                                                                                           \
            perror(name_function);                                                                      \
            exit(EXIT_FAILURE);                                                                         \
        }                                                                                               \
        if (verbose) {                                                                                  \
            printf(GREEN "===== Test réussi ======" RESET "\n");                                        \
        }                                                                                               \
    }                                                                                                   \

// Affiche le manuel d'utilisation serveur
extern void help_serv();

// Affiche le manuel d'utilisation client
extern void help_cli();

// malloc_safe:
//      - Cas d'erreur : Libère les ressources associées à la liste variables de 
//      paramètres et affiche le message d'erreur err_msg  
//      - Cas de succès : renvoie un pointeur vers la mémoire allouée sinon
extern void *malloc_safe(int size, const char *err_msg, int nbptr, ...);
