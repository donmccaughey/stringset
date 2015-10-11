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


void
test_clear(void)
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
    
    int result = stringset_clear(set);
    assert(0 == result);
    assert(0 == set->count);
    
    stringset_free(set);
}


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


void
test_alloc_union(void)
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
    
    struct stringset *set3 = stringset_alloc_union(set1, set2);
    assert(set3);
    
    assert(8 == set3->count);
    
    assert(0 == strcmp("apple", set3->members[0]));
    assert(0 == strcmp("banana", set3->members[1]));
    assert(0 == strcmp("blue", set3->members[2]));
    assert(0 == strcmp("green", set3->members[3]));
    assert(0 == strcmp("mango", set3->members[4]));
    assert(0 == strcmp("red", set3->members[5]));
    assert(0 == strcmp("strawberry", set3->members[6]));
    assert(0 == strcmp("watermelon", set3->members[7]));
    
    stringset_free(set1);
    stringset_free(set2);
    stringset_free(set3);
}


void
test_remove_array(void)
{
    char const *members[] = {
        "watermelon", "mango", "apple", "banana", "strawberry"
    };
    int members_count = sizeof members / sizeof members[0];
    struct stringset *set = stringset_alloc_from_array(members, members_count);
    assert(set);
    
    char const *array[] = {
        "mango", "green", "banana"
    };
    int array_count = sizeof array / sizeof array[0];
    
    int result = stringset_remove_array(set, array, array_count);
    assert(0 == result);
    
    assert(3 == set->count);
    
    assert(0 == strcmp("apple", set->members[0]));
    assert(0 == strcmp("strawberry", set->members[1]));
    assert(0 == strcmp("watermelon", set->members[2]));
    
    stringset_free(set);
}


void
test_remove_stringset(void)
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
    
    int result = stringset_remove_stringset(set1, set2);
    assert(0 == result);
    
    assert(3 == set1->count);
    
    assert(0 == strcmp("apple", set1->members[0]));
    assert(0 == strcmp("strawberry", set1->members[1]));
    assert(0 == strcmp("watermelon", set1->members[2]));
    
    stringset_free(set1);
    stringset_free(set2);
}


void
test_alloc_difference(void)
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
    
    struct stringset *set3 = stringset_alloc_difference(set1, set2);
    assert(set3);
    
    assert(3 == set3->count);
    
    assert(0 == strcmp("apple", set3->members[0]));
    assert(0 == strcmp("strawberry", set3->members[1]));
    assert(0 == strcmp("watermelon", set3->members[2]));
    
    stringset_free(set1);
    stringset_free(set2);
    stringset_free(set3);
}


void
test_is_subset_of(void)
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
    
    assert(!stringset_is_subset_of(set1, set2));
    assert(!stringset_is_subset_of(set1, set3));
    
    assert(!stringset_is_subset_of(set2, set1));
    assert(!stringset_is_subset_of(set2, set3));
    
    assert(stringset_is_subset_of(set3, set1));
    assert(!stringset_is_subset_of(set3, set2));
    
    struct stringset *empty_set = stringset_alloc();
    assert(empty_set);
    
    assert(stringset_is_subset_of(empty_set, set1));
    assert(stringset_is_subset_of(empty_set, set2));
    assert(stringset_is_subset_of(empty_set, set3));
    assert(stringset_is_subset_of(empty_set, empty_set));
    
    assert(!stringset_is_subset_of(set1, empty_set));
    assert(!stringset_is_subset_of(set2, empty_set));
    assert(!stringset_is_subset_of(set3, empty_set));
    
    stringset_free(set1);
    stringset_free(set2);
    stringset_free(set3);
    stringset_free(empty_set);
}


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
    
    assert(!stringset_is_superset_of(set1, set2));
    assert(stringset_is_superset_of(set1, set3));
    
    assert(!stringset_is_superset_of(set2, set1));
    assert(!stringset_is_superset_of(set2, set3));
    
    assert(!stringset_is_superset_of(set3, set1));
    assert(!stringset_is_superset_of(set3, set2));
    
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
    stringset_free(empty_set);
}


void
test_is_equal_to(void)
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
        "banana", "watermelon", "mango"
    };
    int members4_count = sizeof members4 / sizeof members4[0];
    struct stringset *set4 = stringset_alloc_from_array(members4, members4_count);
    assert(set4);
    
    struct stringset *empty_set = stringset_alloc();
    assert(empty_set);
    
    assert(stringset_is_equal_to(set1, set1));
    assert(!stringset_is_equal_to(set1, set2));
    assert(!stringset_is_equal_to(set1, set3));
    assert(!stringset_is_equal_to(set1, set4));
    assert(!stringset_is_equal_to(set1, empty_set));
    
    assert(!stringset_is_equal_to(set2, set1));
    assert(stringset_is_equal_to(set2, set2));
    assert(!stringset_is_equal_to(set2, set3));
    assert(!stringset_is_equal_to(set2, set4));
    assert(!stringset_is_equal_to(set2, empty_set));
    
    assert(!stringset_is_equal_to(set3, set1));
    assert(!stringset_is_equal_to(set3, set2));
    assert(stringset_is_equal_to(set3, set3));
    assert(stringset_is_equal_to(set3, set4));
    assert(!stringset_is_equal_to(set3, empty_set));
    
    assert(!stringset_is_equal_to(set4, set1));
    assert(!stringset_is_equal_to(set4, set2));
    assert(stringset_is_equal_to(set4, set3));
    assert(stringset_is_equal_to(set4, set4));
    assert(!stringset_is_equal_to(set4, empty_set));
    
    assert(!stringset_is_equal_to(empty_set, set1));
    assert(!stringset_is_equal_to(empty_set, set2));
    assert(!stringset_is_equal_to(empty_set, set3));
    assert(!stringset_is_equal_to(empty_set, set4));
    assert(stringset_is_equal_to(empty_set, empty_set));
    
    stringset_free(set1);
    stringset_free(set2);
    stringset_free(set3);
    stringset_free(set4);
    stringset_free(empty_set);
}


void
test_is_disjoint_from(void)
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
    
    assert(!stringset_is_disjoint_from(set1, set1));
    assert(!stringset_is_disjoint_from(set1, set2));
    assert(!stringset_is_disjoint_from(set1, set3));
    assert(stringset_is_disjoint_from(set1, set4));
    assert(stringset_is_disjoint_from(set1, empty_set));
    
    assert(!stringset_is_disjoint_from(set2, set1));
    assert(!stringset_is_disjoint_from(set2, set2));
    assert(!stringset_is_disjoint_from(set2, set3));
    assert(!stringset_is_disjoint_from(set2, set4));
    assert(stringset_is_disjoint_from(set2, empty_set));
    
    assert(!stringset_is_disjoint_from(set3, set1));
    assert(!stringset_is_disjoint_from(set3, set2));
    assert(!stringset_is_disjoint_from(set3, set3));
    assert(stringset_is_disjoint_from(set3, set4));
    assert(stringset_is_disjoint_from(set3, empty_set));
    
    assert(stringset_is_disjoint_from(set4, set1));
    assert(!stringset_is_disjoint_from(set4, set2));
    assert(stringset_is_disjoint_from(set4, set3));
    assert(!stringset_is_disjoint_from(set4, set4));
    assert(stringset_is_disjoint_from(set4, empty_set));
    
    assert(stringset_is_disjoint_from(empty_set, set1));
    assert(stringset_is_disjoint_from(empty_set, set2));
    assert(stringset_is_disjoint_from(empty_set, set3));
    assert(stringset_is_disjoint_from(empty_set, set4));
    assert(stringset_is_disjoint_from(empty_set, empty_set));
    
    stringset_free(set1);
    stringset_free(set2);
    stringset_free(set3);
    stringset_free(set4);
    stringset_free(empty_set);
}


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


void
test_retain_array(void)
{
    char const *members[] = {
        "watermelon", "mango", "apple", "banana", "strawberry"
    };
    int members_count = sizeof members / sizeof members[0];
    struct stringset *set = stringset_alloc_from_array(members, members_count);
    assert(set);
    
    char const *array[] = {
        "mango", "green", "banana"
    };
    int array_count = sizeof array / sizeof array[0];
    
    int result = stringset_retain_array(set, array, array_count);
    assert(0 == result);
    assert(2 == set->count);
    assert(stringset_contains(set, "banana"));
    assert(stringset_contains(set, "mango"));
    
    char const *empty_array[0];
    int empty_array_count = 0;
    result = stringset_retain_array(set, empty_array, empty_array_count);
    assert(0 == result);
    assert(0 == set->count);
    
    stringset_free(set);
}


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
    test_remove_array();
    test_remove_stringset();
    test_alloc_difference();
    test_is_subset_of();
    test_is_superset_of();
    test_is_equal_to();
    test_is_disjoint_from();
    test_alloc_intersection();
    test_retain_array();
    test_retain_stringset();
    return EXIT_SUCCESS;
}
