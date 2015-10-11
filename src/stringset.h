#ifndef STRINGSET_H_INCLUDED
#define STRINGSET_H_INCLUDED


#include <stdbool.h>


struct stringset {
    char **members;
    int count;
};


/****************************
 * Creation and destruction *
 ****************************/

// Allocate an empty string set.
struct stringset *
stringset_alloc(void);

// Allocate a set from an array.
struct stringset *
stringset_alloc_from_array(char const *const *array, int count);

// Delete all members and free an allocated string set.
void
stringset_free(struct stringset *stringset);


/*******************
 * Test membership *
 *******************/

// Check if a string is a member of a string set.
bool
stringset_contains(struct stringset const *stringset, char const *string);

// Check that a string set contains no members in common with another string
// set.
bool
stringset_is_disjoint_from(struct stringset const *stringset,
                           struct stringset const *other);

// Check that a string set contains the same members as another string set
bool
stringset_is_equal_to(struct stringset const *stringset,
                      struct stringset const *other);

// Check if a string set is a subset of another string set.
bool
stringset_is_subset_of(struct stringset const *stringset,
                       struct stringset const *other);

// Check if a string set is a superset of another string set.
bool
stringset_is_superset_of(struct stringset const *stringset,
                         struct stringset const *other);


/*****************************
 * Insert and delete members *
 *****************************/

// Add a string to a string set.  If the string is not a member, a copy is made
// and added to the string set.
int
stringset_add(struct stringset *stringset, char const *string);

// Add an array of strings to a string set.  Each string that is not a member
// is copied and added to the string set.
int
stringset_add_array(struct stringset *stringset,
                    char const *const *array,
                    int count);

// Remove all members from a string set and compact it.
int
stringset_clear(struct stringset *stringset);

// Resize the members array of a string set to free any unused memory.
int
stringset_compact(struct stringset *stringset);

// Remove a string from a string set.
//
// Call `stringset_compact()' after removing members to free unused memory.
int
stringset_remove(struct stringset *stringset, char const *string);

// Remove an array of strings from a string set.
//
// Call `stringset_compact()' after removing members to free unused memory.
int
stringset_remove_array(struct stringset *stringset,
                       char const *const *array,
                       int count);

// Retain only the members of a string set that are present in an array of
// strings.
int
stringset_retain_array(struct stringset *stringset,
                       char const *const *array,
                       int count);


/********************
 * Union operations *
 ********************/

// Allocate a string set that is the union of two string sets.  The allocated
// string set will contain all members of `first' and `second'.
struct stringset *
stringset_alloc_union(struct stringset const *first,
                      struct stringset const *second);

// Add all members of another string set to a string set.  The resulting
// `stringset' will be the union of the original `stringset' and `other'.
int
stringset_add_stringset(struct stringset *stringset,
                        struct stringset const *other);


/***************************
 * Intersection operations *
 ***************************/

// Allocate a string set that is the intersection of two string sets.  The
// allocated string set will contain only members of `first' and `second' that
// are present in both.
struct stringset *
stringset_alloc_intersection(struct stringset const *first,
                             struct stringset const *second);


/*************************
 * Difference operations *
 *************************/

// Allocate a string set that is the difference of two string sets.  The
// allocated string set will contain only members of `first' that are not
// members of `second'.
struct stringset *
stringset_alloc_difference(struct stringset const *first,
                           struct stringset const *second);

// Remove all members of another string set from a string set.  The resulting
// `stringset' will be the difference of the original `stringset' and `other'.
int
stringset_remove_stringset(struct stringset *stringset,
                           struct stringset const *other);


#endif
