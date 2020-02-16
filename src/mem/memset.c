#ifndef MEMSET
#define MEMSET

#include <glob.h>

void *memset(void *s, uint8_t c, size_t n) {
    for (size_t i = 0; i < n; i++) {
        ((char *) s)[i] = (uint8_t) c;
    }
    return s;
}

#endif