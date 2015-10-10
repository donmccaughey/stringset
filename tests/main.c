#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "stringset.h"


void
test_alloc_and_free(void)
{
    struct stringset *set;
    int result = stringset_alloc(&set);
    assert(0 == result);
    assert(set);
    assert(0 == set->count);
    
    stringset_free(&set);
    assert(NULL == set);
}


void
test_add_one_member(void)
{
    struct stringset *set;
    int result = stringset_alloc(&set);
    assert(0 == result);
    
    result = stringset_add(set, "foo");
    assert(0 == result);
    assert(1 == set->count);
    assert(stringset_contains(set, "foo"));
    
    stringset_free(&set);
}


void
test_add_same_member_multiple_times(void)
{
    struct stringset *set;
    int result = stringset_alloc(&set);
    assert(0 == result);
    
    result = stringset_add(set, "foo");
    assert(0 == result);
    assert(1 == set->count);
    assert(stringset_contains(set, "foo"));
    
    result = stringset_add(set, "foo");
    assert(0 == result);
    assert(1 == set->count);
    assert(stringset_contains(set, "foo"));
    
    result = stringset_add(set, "foo");
    assert(0 == result);
    assert(1 == set->count);
    assert(stringset_contains(set, "foo"));
    
    stringset_free(&set);
}


void
test_add_one_hundred_members(void)
{
    struct stringset *set;
    int result = stringset_alloc(&set);
    assert(0 == result);
    
    for (int i = 1; i <= 100; ++i) {
        char *string;
        int chars_formatted = asprintf(&string, "%i", i);
        assert(chars_formatted > 0);
        int result = stringset_add(set, string);
        free(string);
        assert(0 == result);
    }
    
    assert(100 == set->count);
    assert(!stringset_contains(set, "0"));
    assert(stringset_contains(set, "1"));
    assert(stringset_contains(set, "50"));
    assert(stringset_contains(set, "100"));
    assert(!stringset_contains(set, "101"));
    
    stringset_free(&set);
}


int
main(int argc, char *argv[])
{
    test_alloc_and_free();
    test_add_one_member();
    test_add_same_member_multiple_times();
    test_add_one_hundred_members();
    return EXIT_SUCCESS;
}
