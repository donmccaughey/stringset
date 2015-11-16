#include <assert.h>
#include <string.h>

#include "stringset.h"


void
test_alloc_symmetric_difference(void)
{
    char const *members1[] = {
        "watermelon", "mango", "apple", "banana", "strawberry"
    };
    int members1_count = sizeof members1 / sizeof members1[0];
    struct stringset *set1 = stringset_alloc_from_array(members1, members1_count);
    assert(set1);
    
    char const *members2[] = {
        "mango", "green", "banana"
    };
    int members2_count = sizeof members2 / sizeof members2[0];
    struct stringset *set2 = stringset_alloc_from_array(members2, members2_count);
    assert(set2);
    
    struct stringset *set3 = stringset_alloc_symmetric_difference(set1, set2);
    assert(set3);
    
    assert(4 == set3->count);
    
    assert(0 == strcmp("apple", set3->members[0]));
    assert(0 == strcmp("green", set3->members[1]));
    assert(0 == strcmp("strawberry", set3->members[2]));
    assert(0 == strcmp("watermelon", set3->members[3]));
    
    stringset_free(set1);
    stringset_free(set2);
    stringset_free(set3);
}
