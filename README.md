# Obsidian - C testing framework

Just a testing framework built for C.

## Example

```c
#include "obsidian.h"

int add(int a, int b) { return a + b; }

int main(void) {
    OBS_SETUP(TestingAdd)
    // a test group represents a selection of tests
    OBS_TEST_GROUP(FuzzyTests, {
        OBS_TEST("5 + 1", {
            obs_test_eql(add(5, 1), 6)
        })

        // purposefully wrong (10 + 2 != 15) just to show what happens when a test fails
        OBS_TEST("10 + 2", {
            // you can have other stuff here too if you want
            // you can also use this macro to use a different operator
            obs_test(add(10, 2), ==, 15)
        })
    })

    OBS_TEST_GROUP(SameNumbers, {
        OBS_TEST("1 + 1", {
            obs_test_eql(add(1, 1), 2)
        })
    })

    // And so on
    // report the results
    OBS_REPORT
    return 0;
}
```

## Notes

- You can also use asserts inside your code (but shouldn't use `obs_test`) just do `obs_assert(val, op, cond)` i.e. `obs_assert(ptr, !=, NULL);`  These will tell you the evaluation of both sides.
