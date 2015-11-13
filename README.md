stringset
=========

A simple set of strings in C99.

`stringset` is a simple mutable set of strings.  It is implemented using a
sorted array, so it should preform okay for smaller set sizes.  Sorting is
done using `qsort()` and searching by `bsearch()`.  String comparison is done
by `strcmp()`.


Simple Example
--------------
Here is a short code snippet that allocates a `stringset` and adds some strings.

    // allocate a set
    struct stringset *set = stringset_alloc(&set);
    assert(set);


    // add some strings
    result = stringset_add(set, "red");
    assert(0 == result);

    result = stringset_add(set, "green");
    assert(0 == result);

    result = stringset_add(set, "blue");
    assert(0 == result);

    // add "red" again
    result = stringset_add(set, "red");
    assert(0 == result);


    // examine the set
    assert(3 == stringset->count);
    assert(stringset_contains(set, "red"));


    // set members are always sorted
    for (int i = 0; i < set->count; ++i) {
        printf("%s, ", i, set->members[i]);
    }
    // prints "blue, green, red, "


    // clean up
    stringset_free(&set);


License
-------
`stringset` is made available under a BSD-style license; see the LICENSE file 
for details.
