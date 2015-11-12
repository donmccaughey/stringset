#include <assert.h>

#include "stringset.h"


void
test_retain_array(void)
{
    char const *members[] = {
        "watermelon", "mango", "apple", "banana", "strawberry"
    };
    int members_count = sizeof members / sizeof members[0];
    struct stringset *set = stringset_alloc_from_array(members, members_count);
    assert(set);
    
    char const *array[] = {
        "mango", "green", "banana"
    };
    int array_count = sizeof array / sizeof array[0];
    
    int result = stringset_retain_array(set, array, array_count);
    assert(0 == result);
    assert(2 == set->count);
    assert(stringset_contains(set, "banana"));
    assert(stringset_contains(set, "mango"));
    
    char const *empty_array[0];
    int empty_array_count = 0;
    result = stringset_retain_array(set, empty_array, empty_array_count);
    assert(0 == result);
    assert(0 == set->count);
    
    stringset_free(set);
}
