#include "obsidian.h"

int add(int a, int b) { return a + b; }

int main(void) {
    OBS_SETUP(TestingAdd)

    // a test group represents a selection of tests
    OBS_TEST_GROUP(FuzzyTests, {
        OBS_TEST("5 + 1", {
            obs_test_eql(add(5, 1), 6)
        })

        OBS_TEST("10 + 2", {
            obs_test(add(10, 2), ==, 15)
        })
    })

    OBS_TEST_GROUP(SameNumbers, {
        OBS_TEST("1 + 1", {
            obs_test_eql(add(1, 1), 2)
        })
    })

    // And so on
    OBS_REPORT
    return 0;
}