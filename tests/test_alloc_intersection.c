#include <assert.h>

#include "stringset.h"


void
test_alloc_intersection(void)
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
    
    char const *members4[] = {
        "red", "green", "blue"
    };
    int members4_count = sizeof members4 / sizeof members4[0];
    struct stringset *set4 = stringset_alloc_from_array(members4, members4_count);
    assert(set4);
    
    struct stringset *empty_set = stringset_alloc();
    assert(empty_set);
    
    struct stringset *intersection = stringset_alloc_intersection(set1, set2);
    assert(intersection);
    assert(2 == intersection->count);
    assert(stringset_contains(intersection, "banana"));
    assert(stringset_contains(intersection, "mango"));
    stringset_free(intersection);
    
    intersection = stringset_alloc_intersection(set3, set4);
    assert(intersection);
    assert(0 == intersection->count);
    stringset_free(intersection);
    
    intersection = stringset_alloc_intersection(set4, set4);
    assert(intersection);
    assert(3 == intersection->count);
    assert(stringset_contains(intersection, "red"));
    assert(stringset_contains(intersection, "green"));
    assert(stringset_contains(intersection, "blue"));
    stringset_free(intersection);
    
    intersection = stringset_alloc_intersection(set4, empty_set);
    assert(intersection);
    assert(0 == intersection->count);
    stringset_free(intersection);
    
    intersection = stringset_alloc_intersection(empty_set, empty_set);
    assert(intersection);
    assert(0 == intersection->count);
    stringset_free(intersection);
    
    stringset_free(set1);
    stringset_free(set2);
    stringset_free(set3);
    stringset_free(set4);
    stringset_free(empty_set);
}
