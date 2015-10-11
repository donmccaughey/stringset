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


static void
swap(struct stringset *first, struct stringset *second)
{
    struct stringset temp = *first;
    *first = *second;
    *second = temp;
}


struct stringset *
stringset_alloc(void)
{
    return calloc(1, sizeof(struct stringset));
}


struct stringset *
stringset_alloc_difference(struct stringset const *first,
                           struct stringset const *second)
{
    struct stringset *stringset = stringset_alloc();
    if (!stringset) return NULL;
    
    int result = stringset_add_stringset(stringset, first);
    if (-1 == result) {
        stringset_free(stringset);
        return NULL;
    }
    
    result = stringset_remove_stringset(stringset, second);
    if (-1 == result) {
        stringset_free(stringset);
        return NULL;
    }
    
    return stringset;
}


struct stringset *
stringset_alloc_from_array(char const *const *array, int count)
{
    if (!array || count < 0) {
        errno = EINVAL;
        return NULL;
    }
    
    struct stringset *stringset = stringset_alloc();
    if (!stringset) return NULL;
    
    int result = stringset_add_array(stringset, array, count);
    if (-1 == result) {
        stringset_free(stringset);
        return NULL;
    }
    
    return stringset;
}


struct stringset *
stringset_alloc_intersection(struct stringset const *first,
                             struct stringset const *second)
{
    if (!first || !second) {
        errno = EINVAL;
        return NULL;
    }
    
    struct stringset *stringset = stringset_alloc();
    if (!stringset) return NULL;
    
    struct stringset const *smaller;
    struct stringset const *larger;
    if (first->count < second->count) {
        smaller = first;
        larger = second;
    } else {
        smaller = second;
        larger = first;
    }
    
    for (int i = 0; i < smaller->count; ++i) {
        if (stringset_contains(larger, smaller->members[i])) {
            int result = stringset_add(stringset, smaller->members[i]);
            if (-1 == result) {
                stringset_free(stringset);
                return NULL;
            }
        }
    }
    
    return stringset;
}


struct stringset *
stringset_alloc_union(struct stringset const *first,
                      struct stringset const *second)
{
    if (!first || !second) {
        errno = EINVAL;
        return NULL;
    }
    
    struct stringset *stringset = stringset_alloc();
    if (!stringset) return NULL;
    
    int result = stringset_add_stringset(stringset, first);
    if (-1 == result) {
        stringset_free(stringset);
        return NULL;
    }
    
    result = stringset_add_stringset(stringset, second);
    if (-1 == result) {
        stringset_free(stringset);
        return NULL;
    }
    
    return stringset;
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
stringset_free(struct stringset *stringset)
{
    if (stringset) {
        stringset_clear(stringset);
        free(stringset);
    }
}


bool
stringset_is_disjoint_from(struct stringset const *stringset,
                           struct stringset const *other)
{
    if (!stringset || !other) {
        errno = EINVAL;
        return false;
    }
    
    struct stringset const *smaller;
    struct stringset const *larger;
    if (stringset->count < other->count) {
        smaller = stringset;
        larger = other;
    } else {
        smaller = other;
        larger = stringset;
    }
    
    for (int i = 0; i < smaller->count; ++i) {
        if (stringset_contains(larger, smaller->members[i])) return false;
    }
    return true;
}


bool
stringset_is_equal_to(struct stringset const *stringset,
                      struct stringset const *other)
{
    if (!stringset || !other) {
        errno = EINVAL;
        return false;
    }
    
    if (stringset->count != other->count) return false;
    return stringset_is_subset_of(stringset, other);
}


bool
stringset_is_subset_of(struct stringset const *stringset,
                       struct stringset const *other)
{
    if (!stringset || !other) {
        errno = EINVAL;
        return false;
    }
    
    for (int i = 0; i < stringset->count; ++i) {
        if (!stringset_contains(other, stringset->members[i])) return false;
    }
    
    return true;
}


bool
stringset_is_superset_of(struct stringset const *stringset,
                         struct stringset const *other)
{
    return stringset_is_subset_of(other, stringset);
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


int
stringset_retain_array(struct stringset *stringset,
                       char const *const *array,
                       int count)
{
    if (!stringset || !array || count < 0) {
        errno = EINVAL;
        return -1;
    }
    
    struct stringset *other = stringset_alloc_from_array(array, count);
    if (!other) return -1;
    
    int result = stringset_retain_stringset(stringset, other);
    stringset_free(other);
    return result;
}


int
stringset_retain_stringset(struct stringset *stringset,
                           struct stringset const *other)
{
    if (!stringset || !other) {
        errno = EINVAL;
        return -1;
    }
    
    struct stringset *intersection = stringset_alloc_intersection(stringset, other);
    if (!intersection) return -1;
    
    swap(stringset, intersection);
    stringset_free(intersection);
    
    return 0;
}
