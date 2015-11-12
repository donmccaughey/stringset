#include <assert.h>

#include "stringset.h"


void
test_retain_stringset(void)
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
    
    struct stringset *empty_set = stringset_alloc();
    assert(empty_set);
    
    int result = stringset_retain_stringset(set1, set2);
    assert(0 == result);
    assert(2 == set1->count);
    assert(stringset_contains(set1, "banana"));
    assert(stringset_contains(set1, "mango"));
    
    result = stringset_retain_stringset(set1, empty_set);
    assert(0 == result);
    assert(0 == set1->count);
    
    stringset_free(set1);
    stringset_free(set2);
    stringset_free(empty_set);
}
