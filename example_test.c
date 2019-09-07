#include "obsidian.h"

int add(int a, int b) { return a + b; }

int main(int argc, char *argv[]) {
    OBS_SETUP("TestingAdd", argc, argv);
    // a test group represents a selection of tests
    OBS_TEST_GROUP("Fuzzy Tests", {
        OBS_TEST("Basic", {
            obs_test_eq(int, add(5, 1), 6);
            obs_test_neq(int, add(10, 5), 10);
        })

        // purposefully wrong (10 + 2 != 15) just to show what happens when a test fails
        OBS_TEST("10 + 2", {
            // custom test assertion
            obs_test(add(10, 2) == 15, "add(%d, %d) == %d which is not 15", 10, 2, add(10, 2));
        })
    })

    // And so on
    // report the results
    OBS_REPORT;
    return tests_failed;
}