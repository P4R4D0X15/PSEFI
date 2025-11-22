#ifndef HOLDALL__H
#define HOLDALL__H

#include <stdlib.h>

//  struct holdall, holdall : type et nom de type d'un contrôleur regroupant les
//    informations nécessaires pour gérer un fourretout.
typedef struct holdall holdall;

//  holdall_empty : tente d'allouer les ressources nécessaires pour gérer un
//    nouveau fourretout initialement vide. Renvoie un pointeur nul en cas de
//    dépassement de capacité. Renvoie sinon un pointeur vers le contrôleur
//    associé au fourretout.
extern holdall *holdall_empty();

//  holdall_dispose : sans effet si *haptr vaut un pointeur nul. Libère sinon
//    les ressources allouées la gestion du fourretout associé à *haptr puis
//    affecte un pointeur nul à *haptr.
extern void holdall_dispose(holdall **haptr);

//  holdall_put : tente d'insérer ref dans le fourretout associé à ha. Renvoie
//    une valeur non nulle en cas de dépassement de capacité. Renvoie sinon
//    zéro.
extern int holdall_put(holdall *ha, void *ref);

//  holdall_count : renvoie le nombre d'insertions effectuées avec succès dans
//    le fourretout associé à ha depuis sa création.
extern size_t holdall_count(holdall *ha);

//  holdall_apply, holdall_apply_context, holdall_apply_context2 : parcourt le
//    fourretout associé à ha en appelant (respectivement) fun(ref),
//    fun2(ref, fun1(context, ref)), fun2(context2, ref, fun1(context1, ref))
//    pour chacune des références ref dans l'ordre dans lequel elles figurent
//    dans le fourretout. Si, lors du parcours, la valeur de l'appel n'est pas
//    nulle, l'exécution de la fonction prend fin et la fonction renvoie cette
//    valeur. Sinon, la fonction renvoie zéro.
extern int holdall_apply(holdall *ha, int (*fun)(void *));
extern int holdall_apply_context(holdall *ha,
    void *context, void *(*fun1)(void *context, void *ref),
    int (*fun2)(void *ref, void *resultfun1));
extern int holdall_apply_context2(holdall *ha,
    void *context1, void *(*fun1)(void *context1, void *ref),
    void *context2, int (*fun2)(void *context2, void *ref, void *resultfun1));


//  holdall_sort : trie le fourretout associé à ha selon la fonction compar
//    appliquée aux références qui y ont insérées avec succès.
extern void holdall_sort(holdall *ha,
    int (*compar)(const void *, const void *));

#endif
