#ifndef STRINGSET_H_INCLUDED
#define STRINGSET_H_INCLUDED


#include <stdbool.h>


struct stringset {
    char **members;
    int count;
};


int
stringset_alloc(struct stringset **stringset);

void
stringset_free(struct stringset **stringset);

int
stringset_add(struct stringset *stringset, char const *string);

bool
stringset_contains(struct stringset *stringset, char const *string);


#endif
