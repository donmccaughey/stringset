#include <assert.h>
#include <string.h>

#include "stringset.h"


void
test_add_array(void)
{
    struct stringset *set = stringset_alloc();
    assert(set);
    
    char const *members[] = {
        "watermelon", "mango", "apple", "banana", "strawberry"
    };
    int members_count = sizeof members / sizeof members[0];
    
    int result = stringset_add_array(set, members, members_count);
    assert(0 == result);
    
    assert(5 == set->count);
    
    assert(0 == strcmp("apple", set->members[0]));
    assert(0 == strcmp("banana", set->members[1]));
    assert(0 == strcmp("mango", set->members[2]));
    assert(0 == strcmp("strawberry", set->members[3]));
    assert(0 == strcmp("watermelon", set->members[4]));
    
    stringset_free(set);
}
