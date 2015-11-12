#include <assert.h>
#include <string.h>

#include "stringset.h"


void
test_remove_array(void)
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
    
    int result = stringset_remove_array(set, array, array_count);
    assert(0 == result);
    
    assert(3 == set->count);
    
    assert(0 == strcmp("apple", set->members[0]));
    assert(0 == strcmp("strawberry", set->members[1]));
    assert(0 == strcmp("watermelon", set->members[2]));
    
    stringset_free(set);
}
