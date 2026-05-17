#ifndef KS_IO_H
#define KS_IO_H

#include <ks/core.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

KS_API char* ks_fread(const char* filename);

#endif  // KS_IO_H

#if defined(KS_IO_IMPL) && !defined(KS_IO_IMPL_DONE)
#define KS_IO_IMPL_DONE

#if !defined(KS_CORE_IMPL) && !defined(KS_CORE_IMPL_DONE)
#error "kitsune: io.h requires core.h"
#endif

KS_API char* ks_fread(const char* filename) {
    FILE* f = fopen(filename, "r");
    if (!f) {
        return NULL;
    }

    fseek(f, 0l, SEEK_END);
    size_t size = (size_t)ftell(f);
    rewind(f);

    char* content = malloc(size + 1);
    if (!content) {
        fclose(f);
        return NULL;
    }

    fread(content, 1, size, f);
    content[size] = '\0';

    fclose(f);

    return content;
}

#endif  // KS_IO_IMPL