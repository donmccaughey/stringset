#include <assert.h>
#include <string.h>

#include "stringset.h"


void
test_add_stringset(void)
{
    char const *members1[] = {
        "watermelon", "mango", "apple", "banana", "strawberry"
    };
    int members1_count = sizeof members1 / sizeof members1[0];
    struct stringset *set1 = stringset_alloc_from_array(members1, members1_count);
    assert(set1);
    
    char const *members2[] = {
        "red", "green", "blue"
    };
    int members2_count = sizeof members2 / sizeof members2[0];
    struct stringset *set2 = stringset_alloc_from_array(members2, members2_count);
    assert(set2);
    
    int result = stringset_add_stringset(set1, set2);
    assert(0 == result);
    
    assert(8 == set1->count);
    
    assert(0 == strcmp("apple", set1->members[0]));
    assert(0 == strcmp("banana", set1->members[1]));
    assert(0 == strcmp("blue", set1->members[2]));
    assert(0 == strcmp("green", set1->members[3]));
    assert(0 == strcmp("mango", set1->members[4]));
    assert(0 == strcmp("red", set1->members[5]));
    assert(0 == strcmp("strawberry", set1->members[6]));
    assert(0 == strcmp("watermelon", set1->members[7]));
    
    stringset_free(set1);
    stringset_free(set2);
}
