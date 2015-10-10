#include "stringset.h"

#include <errno.h>
#include <stdlib.h>
#include <string.h>


static int
compare_strings(void const *first, void const *second)
{
    char *const *first_string = first;
    char *const *second_string = second;
    if (!*first_string && !*second_string) return 0;
    if (!*first_string) return 1;
    if (!*second_string) return -1;
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


int
stringset_add_array(struct stringset *stringset,
                    char const **array,
                    int count)
{
    if (!stringset || !array || count < 0) {
        errno = EINVAL;
        return -1;
    }
    
    if (!count) return 0;
    
    for (int i = 0; i < count; ++i) {
        int result = stringset_add(stringset, array[i]);
        if (-1 == result) return -1;
    }
    
    return 0;
}


int
stringset_add_stringset(struct stringset *stringset,
                        struct stringset const *other_stringset)
{
    if (!stringset || !other_stringset) {
        errno = EINVAL;
        return -1;
    }
    
    if (!other_stringset->count) return 0;
    
    for (int i = 0; i < other_stringset->count; ++i) {
        int result = stringset_add(stringset, other_stringset->members[i]);
        if (-1 == result) return -1;
    }
    
    return 0;
}


int
stringset_clear(struct stringset *stringset)
{
    if (!stringset) {
        errno = EINVAL;
        return -1;
    }
    
    free(stringset->members);
    stringset->count = 0;
    stringset->members = NULL;
    
    return 0;
}


int
stringset_compact(struct stringset *stringset)
{
    if (!stringset) {
        errno = EINVAL;
        return -1;
    }
    
    size_t new_size = sizeof(char *) * stringset->count;
    char **new_members = realloc(stringset->members, new_size);
    if (!new_members) return -1;
    
    stringset->members = new_members;
    
    return 0;
}


bool
stringset_contains(struct stringset const *stringset, char const *string)
{
    if (!stringset || !string) {
        errno = EINVAL;
        return false;
    }
    
    void *found = bsearch(&string,
                          stringset->members,
                          stringset->count,
                          sizeof(char *),
                          compare_strings);
    return found ? true : false;
}


int
stringset_remove(struct stringset *stringset, char const *string)
{
    if (!stringset || !string) {
        errno = EINVAL;
        return -1;
    }
    
    void *found = bsearch(&string,
                          stringset->members,
                          stringset->count,
                          sizeof(char *),
                          compare_strings);
    if (found) {
        char **member = found;
        *member = NULL;
        
        qsort(stringset->members,
              stringset->count,
              sizeof(char *),
              compare_strings);
        
        --stringset->count;
    }
    
    return 0;
}
