#include <semaphore.h>

#define RED     "\x1b[31m"
#define GREEN   "\x1b[32m"
#define YELLOW  "\x1b[33m"
#define BLUE    "\x1b[34m"
#define MAGENTA "\x1b[35m"
#define CYAN    "\x1b[36m"
#define RESET   "\x1b[0m"
#define FLASH   "\x1b[5m"

#define CHECK_RETURN(arg, err_val, name_function)                                                       \
    {                                                                                                   \
        if (arg == err_val) {                                                                           \
            perror(name_function);                                                                      \
            exit(EXIT_FAILURE);                                                                         \
        }                                                                                               \
    }                                                                                                   \

// Affiche le manuel d'utilisation serveur
extern void help_serv();

// Affiche le manuel d'utilisation client
extern void help_cli();

// sighandler : 
extern void sighandler(int sig);