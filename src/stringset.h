#ifndef STRINGSET_H_INCLUDED
#define STRINGSET_H_INCLUDED


#include <stdbool.h>


struct stringset {
    char **members;
    int count;
};


int
stringset_alloc(struct stringset **stringset);

int
stringset_alloc_union(struct stringset **stringset,
                      struct stringset const *first_other,
                      struct stringset const *second_other);

void
stringset_free(struct stringset **stringset);

int
stringset_add(struct stringset *stringset, char const *string);

int
stringset_add_array(struct stringset *stringset,
                    char const **array,
                    int count);

int
stringset_add_stringset(struct stringset *stringset,
                        struct stringset const *other);

int
stringset_clear(struct stringset *stringset);

int
stringset_compact(struct stringset *stringset);

bool
stringset_contains(struct stringset const *stringset, char const *string);

int
stringset_remove(struct stringset *stringset, char const *string);


#endif
