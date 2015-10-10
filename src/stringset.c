#include "stringset.h"

#include <errno.h>
#include <stdlib.h>
#include <string.h>


static int
compare_strings(void const *first, void const *second)
{
    char * const *first_string = first;
    char * const *second_string = second;
    return strcmp(*first_string, *second_string);
}


int
stringset_alloc(struct stringset **stringset)
{
    if (!stringset) {
        errno = EINVAL;
        return -1;
    }
    
    *stringset = calloc(1, sizeof(struct stringset));
    return *stringset ? 0 : -1;
}


void
stringset_free(struct stringset **stringset)
{
    if (!stringset) return;
    
    if (*stringset) {
        if ((*stringset)->members) {
            for (int i = 0; i < (*stringset)->count; ++i) {
                free((*stringset)->members[i]);
            }
            free((*stringset)->members);
        }
        free(*stringset);
    }
    
    *stringset = NULL;
}


int
stringset_add(struct stringset *stringset, char const *string)
{
    if (!stringset || !string) {
        errno = EINVAL;
        return -1;
    }
    
    if (stringset_contains(stringset, string)) return 0;
    
    int new_index = stringset->count;
    int new_count = stringset->count + 1;
    size_t new_size = sizeof(char *) * new_count;
    char **new_members = realloc(stringset->members, new_size);
    if (!new_members) return -1;
    
    stringset->members = new_members;
    stringset->members[new_index] = strdup(string);
    if (!stringset->members[new_index]) return -1;
    
    stringset->count = new_count;
    
    qsort(stringset->members,
          stringset->count,
          sizeof(char *),
          compare_strings);
    
    return 0;
}


bool
stringset_contains(struct stringset *stringset, char const *string)
{
    if (!stringset || !string) {
        errno = EINVAL;
        return false;
    }
    
    void *member = bsearch(&string,
                           stringset->members,
                           stringset->count,
                           sizeof(char *),
                           compare_strings);
    return member ? true : false;
}
