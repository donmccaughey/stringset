#include <assert.h>

#include "stringset.h"


void
test_remove(void)
{
    char const *members[] = {
        "watermelon", "mango", "apple", "banana", "strawberry"
    };
    int members_count = sizeof members / sizeof members[0];
    struct stringset *set = stringset_alloc_from_array(members, members_count);
    assert(set);
    
    int result = stringset_remove(set, "banana");
    assert(0 == result);
    assert(4 == set->count);
    assert(!stringset_contains(set, "banana"));
    
    result = stringset_remove(set, "watermelon");
    assert(0 == result);
    assert(3 == set->count);
    assert(!stringset_contains(set, "watermelon"));
    
    result = stringset_remove(set, "apple");
    assert(0 == result);
    assert(2 == set->count);
    assert(!stringset_contains(set, "apple"));
    
    result = stringset_remove(set, "mango");
    assert(0 == result);
    assert(1 == set->count);
    assert(!stringset_contains(set, "mango"));
    
    result = stringset_remove(set, "strawberry");
    assert(0 == result);
    assert(0 == set->count);
    assert(!stringset_contains(set, "strawberry"));
    
    result = stringset_compact(set);
    assert(0 == result);
    
    stringset_free(set);
}
