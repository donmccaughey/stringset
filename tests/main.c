#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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


void
test_members_are_sorted(void)
{
    struct stringset *set;
    int result = stringset_alloc(&set);
    assert(0 == result);
    
    result = stringset_add(set, "watermelon");
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
    
    stringset_free(&set);
}


void
test_remove(void)
{
    struct stringset *set;
    int result = stringset_alloc(&set);
    assert(0 == result);
    
    result = stringset_add(set, "watermelon");
    assert(0 == result);
    
    result = stringset_add(set, "mango");
    assert(0 == result);
    
    result = stringset_add(set, "apple");
    assert(0 == result);
    
    result = stringset_add(set, "banana");
    assert(0 == result);
    
    result = stringset_add(set, "strawberry");
    assert(0 == result);
    
    result = stringset_remove(set, "banana");
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
    
    stringset_free(&set);
}


void
test_clear(void)
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
    
    result = stringset_clear(set);
    assert(0 == result);
    assert(0 == set->count);
    
    stringset_free(&set);
}


void
test_add_array(void)
{
    struct stringset *set;
    int result = stringset_alloc(&set);
    assert(0 == result);
    
    char const *members[] = {
        "watermelon", "mango", "apple", "banana", "strawberry"
    };
    int members_count = sizeof members / sizeof members[0];
    
    result = stringset_add_array(set, members, members_count);
    assert(0 == result);
    
    assert(5 == set->count);
    
    assert(0 == strcmp("apple", set->members[0]));
    assert(0 == strcmp("banana", set->members[1]));
    assert(0 == strcmp("mango", set->members[2]));
    assert(0 == strcmp("strawberry", set->members[3]));
    assert(0 == strcmp("watermelon", set->members[4]));
    
    stringset_free(&set);
}


void
test_add_stringset(void)
{
    struct stringset *set1;
    int result = stringset_alloc(&set1);
    assert(0 == result);
    
    char const *members1[] = {
        "watermelon", "mango", "apple", "banana", "strawberry"
    };
    int members1_count = sizeof members1 / sizeof members1[0];
    
    result = stringset_add_array(set1, members1, members1_count);
    assert(0 == result);

    struct stringset *set2;
    result = stringset_alloc(&set2);
    assert(0 == result);
    
    char const *members2[] = {
        "red", "green", "blue"
    };
    int members2_count = sizeof members2 / sizeof members2[0];
    
    result = stringset_add_array(set2, members2, members2_count);
    assert(0 == result);
    
    result = stringset_add_stringset(set1, set2);
    
    assert(8 == set1->count);
    
    assert(0 == strcmp("apple", set1->members[0]));
    assert(0 == strcmp("banana", set1->members[1]));
    assert(0 == strcmp("blue", set1->members[2]));
    assert(0 == strcmp("green", set1->members[3]));
    assert(0 == strcmp("mango", set1->members[4]));
    assert(0 == strcmp("red", set1->members[5]));
    assert(0 == strcmp("strawberry", set1->members[6]));
    assert(0 == strcmp("watermelon", set1->members[7]));
    
    stringset_free(&set1);
    stringset_free(&set2);
}


void
test_alloc_union(void)
{
    struct stringset *set1;
    int result = stringset_alloc(&set1);
    assert(0 == result);
    
    char const *members1[] = {
        "watermelon", "mango", "apple", "banana", "strawberry"
    };
    int members1_count = sizeof members1 / sizeof members1[0];
    
    result = stringset_add_array(set1, members1, members1_count);
    assert(0 == result);
    
    struct stringset *set2;
    result = stringset_alloc(&set2);
    assert(0 == result);
    
    char const *members2[] = {
        "red", "green", "blue"
    };
    int members2_count = sizeof members2 / sizeof members2[0];
    
    result = stringset_add_array(set2, members2, members2_count);
    assert(0 == result);
    
    struct stringset *set3;
    result = stringset_alloc_union(&set3, set1, set2);
    
    assert(8 == set3->count);
    
    assert(0 == strcmp("apple", set3->members[0]));
    assert(0 == strcmp("banana", set3->members[1]));
    assert(0 == strcmp("blue", set3->members[2]));
    assert(0 == strcmp("green", set3->members[3]));
    assert(0 == strcmp("mango", set3->members[4]));
    assert(0 == strcmp("red", set3->members[5]));
    assert(0 == strcmp("strawberry", set3->members[6]));
    assert(0 == strcmp("watermelon", set3->members[7]));
    
    stringset_free(&set1);
    stringset_free(&set2);
    stringset_free(&set3);
}


int
main(int argc, char *argv[])
{
    test_alloc_and_free();
    test_add_one_member();
    test_add_same_member_multiple_times();
    test_add_one_hundred_members();
    test_members_are_sorted();
    test_remove();
    test_clear();
    test_add_array();
    test_add_stringset();
    test_alloc_union();
    return EXIT_SUCCESS;
}
