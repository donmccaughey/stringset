#include <assert.h>

#include "stringset.h"


void
test_is_superset_of(void)
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
    
    char const *members3[] = {
        "mango", "watermelon", "banana"
    };
    int members3_count = sizeof members3 / sizeof members3[0];
    struct stringset *set3 = stringset_alloc_from_array(members3, members3_count);
    assert(set3);
    
    struct stringset *set3_dup = stringset_alloc_from_stringset(set3);
    assert(set3_dup);
    
    assert(!stringset_is_superset_of(set1, set2));
    assert(stringset_is_superset_of(set1, set3));
    
    assert(!stringset_is_superset_of(set2, set1));
    assert(!stringset_is_superset_of(set2, set3));
    
    assert(!stringset_is_superset_of(set3, set1));
    assert(!stringset_is_superset_of(set3, set2));
    
    assert(stringset_is_superset_of(set3, set3));
    assert(stringset_is_superset_of(set3, set3_dup));
    assert(stringset_is_superset_of(set3_dup, set3));
    
    struct stringset *empty_set = stringset_alloc();
    assert(empty_set);
    
    assert(!stringset_is_superset_of(empty_set, set1));
    assert(!stringset_is_superset_of(empty_set, set2));
    assert(!stringset_is_superset_of(empty_set, set3));
    assert(stringset_is_superset_of(empty_set, empty_set));
    
    assert(stringset_is_superset_of(set1, empty_set));
    assert(stringset_is_superset_of(set2, empty_set));
    assert(stringset_is_superset_of(set3, empty_set));
    
    stringset_free(set1);
    stringset_free(set2);
    stringset_free(set3);
    stringset_free(set3_dup);
    stringset_free(empty_set);
}
