#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "stringset.h"


void
test_alloc_and_free(void)
{
    struct stringset *set = stringset_alloc();
    assert(set);
    assert(0 == set->count);
    
    stringset_free(set);
}


void
test_add_one_member(void)
{
    struct stringset *set = stringset_alloc();
    assert(set);
    
    int result = stringset_add(set, "foo");
    assert(0 == result);
    assert(1 == set->count);
    assert(stringset_contains(set, "foo"));
    
    stringset_free(set);
}


void
test_add_same_member_multiple_times(void)
{
    struct stringset *set = stringset_alloc();
    assert(set);
    
    int result = stringset_add(set, "foo");
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
    
    stringset_free(set);
}


void
test_add_one_hundred_members(void)
{
    struct stringset *set = stringset_alloc();
    assert(set);
    
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
    
    stringset_free(set);
}


void
test_members_are_sorted(void)
{
    struct stringset *set = stringset_alloc();
    assert(set);
    
    int result = stringset_add(set, "watermelon");
    assert(0 == result);
    
    result = stringset_add(set, "mango");
    assert(0 == result);
    
    result = stringset_add(set, "apple");
    assert(0 == result);
    
    result = stringset_add(set, "banana");
    assert(0 == result);
    
    result = stringset_add(set, "strawberry");
    assert(0 == result);
    
    assert(5 == set->count);
    
    assert(0 == strcmp("apple", set->members[0]));
    assert(0 == strcmp("banana", set->members[1]));
    assert(0 == strcmp("mango", set->members[2]));
    assert(0 == strcmp("strawberry", set->members[3]));
    assert(0 == strcmp("watermelon", set->members[4]));
    
    stringset_free(set);
}


void
test_add_array(void);

void
test_add_stringset(void);

void
test_alloc_difference(void);

void
test_alloc_intersection(void);

void
test_alloc_union(void);

void
test_clear(void);

void
test_is_disjoint_from(void);

void
test_is_equal_to(void);

void
test_is_proper_subset_of(void);

void
test_is_proper_superset_of(void);

void
test_is_subset_of(void);

void
test_is_superset_of(void);

void
test_remove(void);

void
test_remove_array(void);

void
test_remove_stringset(void);

void
test_retain_array(void);

void
test_retain_stringset(void);


int
main(int argc, char *argv[])
{
    test_alloc_and_free();
    test_add_one_member();
    test_add_same_member_multiple_times();
    test_add_one_hundred_members();
    test_members_are_sorted();
    
    test_add_array();
    test_add_stringset();
    test_alloc_difference();
    test_alloc_intersection();
    test_alloc_union();
    test_clear();
    test_is_disjoint_from();
    test_is_equal_to();
    test_is_proper_subset_of();
    test_is_proper_superset_of();
    test_is_subset_of();
    test_is_superset_of();
    test_remove();
    test_remove_array();
    test_remove_stringset();
    test_retain_array();
    test_retain_stringset();
    
    return EXIT_SUCCESS;
}
