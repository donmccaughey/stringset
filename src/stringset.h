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
int
stringset_alloc(struct stringset **stringset);

// Delete all members and free an allocated string set.
void
stringset_free(struct stringset **stringset);


/*******************
 * Test membership *
 *******************/

// Check if a string is a member of a string set.
bool
stringset_contains(struct stringset const *stringset, char const *string);

// Check if a string set is a subset of another string set.
bool
stringset_is_subset_of(struct stringset const *stringset,
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


/********************
 * Union operations *
 ********************/

// Allocate a string set that is the union of two string sets.  The resulting
// `stringset' will contain all members of `first' and `second'.
int
stringset_alloc_union(struct stringset **stringset,
                      struct stringset const *first,
                      struct stringset const *second);

// Add all members of another string set to a string set.  The resulting
// `stringset' will be the union of the original `stringset' and `other'.
int
stringset_add_stringset(struct stringset *stringset,
                        struct stringset const *other);


/*************************
 * Difference operations *
 *************************/

// Allocate a string set that is the difference of two string sets.
// The resulting `stringset' will contain only members of `first' that are
// not members of `second'.
int
stringset_alloc_difference(struct stringset **stringset,
                           struct stringset const *first,
                           struct stringset const *second);

// Remove all members of another string set from a string set.  The resulting
// `stringset' will be the difference of the original `stringset' and `other'.
int
stringset_remove_stringset(struct stringset *stringset,
                           struct stringset const *other);


#endif
