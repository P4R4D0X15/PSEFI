//  holdall.c : partie implantation du module holdall.

#include "holdall.h"
#include <stdio.h>

#define CAPACITY_MIN 512
#define CAPACITY_MUL 2

#define VALUE_ALREADY_EXIST 1
#define LACK_OF_MEMORY -1

struct holdall {
  void **aref;
  size_t cap;
  size_t count;
};

holdall *holdall_empty() {
  holdall *ha = malloc(sizeof *ha);
  if (ha == nullptr) {
    return nullptr;
  }
  ha->aref = malloc(CAPACITY_MIN * sizeof(*(ha->aref)));
  if (ha->aref == nullptr) {
    free(ha);
    return nullptr;
  }
  ha->cap = CAPACITY_MIN;
  ha->count = 0;
  return ha;
}

void holdall_dispose(holdall **haptr) {
  if (*haptr == nullptr) {
    return;
  }
  free((*haptr)->aref);
  free(*haptr);
  *haptr = nullptr;
}

int holdall_put(holdall *ha, void *ref) {
  if (ha->count == ha->cap) {
    void **new_array = realloc(ha->aref,
          CAPACITY_MUL * ha->cap * sizeof(*(ha->aref)));
    if (new_array == nullptr) {
      return LACK_OF_MEMORY;
    }
    ha->cap *= CAPACITY_MUL;
    ha->aref = new_array;
  }
  ha->aref[ha->count] = ref;
  ha->count += 1;
  return 0;
}

size_t holdall_count(holdall *ha) {
  return ha->count;
}

int holdall_apply(holdall *ha,
    int (*fun)(void *)) {
  for (size_t i = 0; i < ha->count; ++i) {
    int r = fun(ha->aref[i]);
    if (r != 0) {
      return r;
    }
  }
  return 0;
}

int holdall_apply_context(holdall *ha,
    void *context, void *(*fun1)(void *context, void *ptr),
    int (*fun2)(void *ptr, void *resultfun1)) {
  for (size_t i = 0; i < ha->count; ++i) {
    int r = fun2(ha->aref[i], fun1(context, ha->aref[i]));
    if (r != 0) {
      return r;
    }
  }
  return 0;
}

int holdall_apply_context2(holdall *ha,
    void *context1, void *(*fun1)(void *context1, void *ptr),
    void *context2, int (*fun2)(void *context2, void *ptr, void *resultfun1)) {
  for (size_t i = 0; i < ha->count; ++i) {
    int r = fun2(context2, ha->aref[i], fun1(context1, ha->aref[i]));
    if (r != 0) {
      return r;
    }
  }
  return 0;
}

#if defined WANT_HOLDALL_EXT && defined HOLDALL_EXT

// mem_swap : prend en paramètres l'adresse de deux blocs mémoires et échange
// les deux zones mémoires de taille size pointées par s1 et s2
static void mem_swap(void *s1, void *s2, size_t size) {
  char *p1 = s1;
  char *p2 = s2;
  // IB : 0 <= k && k<= size
  //      && p1 == (char *)s1 + k
  //      && p2 == (char *) s2 + k
  //      && le préfixe de longueur k de s1 et s2 ont été  échangés
  //      && les suffixes complémentaires n'ont pas été modifiés
  // QC : k
  for (size_t k = 0; k < size; ++k) {
    int temp = *p1;
    *p1 = *p2;
    *p2 = (char) temp;
    ++p1;
    ++p2;
  }
}

//  heapsort_down : il est supposé que base est l'adresse du premier composant
//    d'un tableau de longueur nmemb et de taille de composants size, que
//    nmemb >= 1, que k <= nmemb - 1 et que le tableau est un maximier sur
//    [ k + 1 ... nmemb - 1 ] relativement à la fonction de comparaison pointée
//    par compar. Descend le composant d'indice k à la bonne place de manière à
//    faire du tableau un maximier sur [ k ... nmemb - 1 ].
static void heapsort_down(char *base, size_t nmemb, size_t size,
    int (*compar)(const void *, const void *), size_t k) {
  size_t i = k;
  while (true) {
    size_t gauche = 2 * i + 1;
    size_t droite = 2 * i + 2;
    size_t biggest = i;
    if (gauche < nmemb && compar(base + gauche * size,
          base + biggest * size) > 0) {
      biggest = gauche;
    }
    if (droite < nmemb && compar(base + droite * size,
          base + biggest * size) > 0) {
      biggest = droite;
    }
    if (biggest == i) {
      break;
    }
    mem_swap(base + i * size, base + biggest * size, size);
    i = biggest;
  }
}

//  heapsort : même spécification que la fonction qsort déclarée dans l'en-tête
//    <stdlib.h> de la bibliothèque standard.
static void heapsort(void *base, size_t nmemb, size_t size,
    int (*compar)(const void *, const void *)) {
  for (size_t k = nmemb / 2; k > 0; k--) {
    heapsort_down(base, nmemb, size, compar, k - 1);
  }
  while (nmemb > 1) {
    mem_swap(base, (char *) base + (nmemb - 1) * size, size);
    nmemb--;
    heapsort_down(base, nmemb, size, compar, 0);
  }
}

extern void holdall_sort(holdall *ha,
    int (*compar)(const void *, const void *)) {
  heapsort(ha, ha->count, 1, compar);
}

#endif
