#include "log.h"

system_logs *empty_logs(){
    system_logs *slogs = malloc(sizeof(system_logs));
    if (slogs == nullptr)
        return nullptr;

    slogs->alogs = malloc(sizeof(*(slogs->alogs)) * CAP_MIN);
    if (slogs->alogs == nullptr) {
        free(slogs);
        return nullptr;
    }

    slogs->count  = 0;
    slogs->length = CAP_MIN;
    return slogs;
}

char *search(const char *path, int filter, system_logs *slogs) {
    if (path == nullptr || slogs == nullptr) {
        return nullptr;
    }
    for (size_t i = 0; i < slogs->count; ++i) {
        log *l = slogs->alogs[i];
        if (l != nullptr && l->filter == filter 
            && strcmp(l->origin, path) == 0) {
            return l->filtered_img;
        }
    }
    return nullptr;
}

void *append_log(const char *origin_path, int filter, const char *filtered_path, system_logs *slogs) {
    if (origin_path == nullptr || filtered_path == nullptr || slogs == nullptr){
        return nullptr;
    }
    if (slogs->count == slogs->length) {
        size_t new_len = slogs->length * CAP_MUL;
        log **tmp = realloc(slogs->alogs, sizeof(*tmp) * new_len);
        if (tmp == nullptr){
            return nullptr;
        }
        slogs->alogs  = tmp;
        slogs->length = new_len;
    }

    log *l = malloc(sizeof(log));
    if (l == nullptr)
        return nullptr;

    strncpy(l->origin, origin_path, PATH_SIZE - 1);
    l->origin[PATH_SIZE - 1] = '\0';

    strncpy(l->filtered_img, filtered_path, PATH_SIZE - 1);
    l->filtered_img[PATH_SIZE - 1] = '\0';

    l->filter = filter;

    slogs->alogs[slogs->count] = l;
    slogs->count += 1;
    return l;
}

void clear_logs(system_logs **slogs) {
    if (slogs == nullptr || *slogs == nullptr){
        return;
    }
    for (size_t i = 0; i < (*slogs)->count; ++i) {
        log *l = (*slogs)->alogs[i];
        if (l != nullptr) {
            free(l);
        }
    }
    free((*slogs)->alogs);
    free(*slogs);
    *slogs = nullptr;
}
