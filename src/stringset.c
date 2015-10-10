#include "stringset.h"

#include <errno.h>
#include <stdlib.h>
#include <string.h>


static int
add_array(struct stringset *stringset,
          char const *const *array,
          int count)
{
    if (!count) return 0;
    
    for (int i = 0; i < count; ++i) {
        int result = stringset_add(stringset, array[i]);
        if (-1 == result) return -1;
    }
    
    return 0;
}


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


static int
remove_array(struct stringset *stringset,
             char const *const *array,
             int count)
{
    for (int i = 0; i < count; ++i) {
        int result = stringset_remove(stringset, array[i]);
        if (-1 == result) return -1;
    }
    
    int result = stringset_compact(stringset);
    if (-1 == result) return -1;
    
    return 0;
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


int
stringset_alloc_difference(struct stringset **stringset,
                           struct stringset const *first,
                           struct stringset const *second)
{
    int result = stringset_alloc(stringset);
    if (-1 == result) return -1;
    
    result = stringset_add_stringset(*stringset, first);
    if (-1 == result) {
        stringset_free(stringset);
        return -1;
    }
    
    result = stringset_remove_stringset(*stringset, second);
    if (-1 == result) {
        stringset_free(stringset);
        return -1;
    }
    
    return 0;
}


int
stringset_alloc_union(struct stringset **stringset,
                      struct stringset const *first,
                      struct stringset const *second)
{
    int result = stringset_alloc(stringset);
    if (-1 == result) return -1;
    
    result = stringset_add_stringset(*stringset, first);
    if (-1 == result) {
        stringset_free(stringset);
        return -1;
    }
    
    result = stringset_add_stringset(*stringset, second);
    if (-1 == result) {
        stringset_free(stringset);
        return -1;
    }
    
    return 0;
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
                    char const *const *array,
                    int count)
{
    if (!stringset || !array || count < 0) {
        errno = EINVAL;
        return -1;
    }
    
    return add_array(stringset, array, count);
}


int
stringset_add_stringset(struct stringset *stringset,
                        struct stringset const *other)
{
    if (!stringset || !other) {
        errno = EINVAL;
        return -1;
    }
    
    return add_array(stringset,
                     (char const *const *)other->members,
                     other->count);
}


int
stringset_clear(struct stringset *stringset)
{
    if (!stringset) {
        errno = EINVAL;
        return -1;
    }
    
    for (int i = 0; i < stringset->count; ++i) {
        free(stringset->members[i]);
    }
    stringset->count = 0;
    stringset_compact(stringset);
    
    return 0;
}


int
stringset_compact(struct stringset *stringset)
{
    if (!stringset) {
        errno = EINVAL;
        return -1;
    }
    
    if (stringset->count) {
        size_t new_size = sizeof(char *) * stringset->count;
        char **new_members = realloc(stringset->members, new_size);
        if (!new_members) return -1;
        stringset->members = new_members;
    } else {
        free(stringset->members);
        stringset->members = NULL;
    }
    
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


void
stringset_free(struct stringset **stringset)
{
    if (!stringset) return;
    
    if (*stringset) {
        stringset_clear(*stringset);
        free(*stringset);
    }
    
    *stringset = NULL;
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


int
stringset_remove_array(struct stringset *stringset,
                       char const *const *array,
                       int count)
{
    if (!stringset || !array || count < 0) {
        errno = EINVAL;
        return -1;
    }
    
    return remove_array(stringset, array, count);
}


int
stringset_remove_stringset(struct stringset *stringset,
                           struct stringset const *other)
{
    if (!stringset || !other) {
        errno = EINVAL;
        return -1;
    }
    
    return remove_array(stringset,
                        (char const *const *)other->members,
                        other->count);
}
