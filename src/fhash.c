#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <assert.h>
#include <err.h>
#include <string.h>

#include "glean.h"
#include "set.h"
#include "whash.h"
#include "fhash.h"

hash_t fname_hash(void *v) {
    return hash_word(((fname *)v)->name);
}

static int fname_cmp(void *a, void *b) {
    char *na = ((fname*)a)->name, *nb = ((fname*)b)->name;
    
    if (DEBUG) fprintf(stderr, "cmp: %s / %s -> %d\n",
        na, nb, strcmp(na, nb));
    
    return strcmp(na, nb);
}

set *init_fname_set(int sz_factor) {
    return set_init(sz_factor, fname_hash, fname_cmp);
}

fname *new_fname(char *n, size_t len) {
    fname *res = alloc(sizeof(fname), 'f');
    char *name = alloc(len + 1, 'n');
    strncpy(name, n, len + 1); /* strlcpy */
    res->name = name;
    return res;
}

void free_fname(void *f) {
    fname *fn = (fname *)f;
    free(fn->name);
    free(fn);
}

fname *add_fname(set *s, fname *f) {
    int res;
    if (DEBUG) fprintf(stderr,
        "Adding filename %s (%ld)\n", f->name, strlen(f->name));
    res = set_store(s, f);
    if (res == TABLE_SET_FAIL) err(1, "set_store failure");
    return f;
}
