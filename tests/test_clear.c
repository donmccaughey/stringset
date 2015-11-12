#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "stringset.h"


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
