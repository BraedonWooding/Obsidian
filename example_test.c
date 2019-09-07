#define OBS_GET_TIME cbenchGetTime
#define OBS_GET_WALL_TIME cbenchGetWallTime
#define obsTime cbenchTime

#define OBS_NO_BENCHMARKS_CHILD

#include "obsidian.h"

// note how this is below obsidian which means it won't detect cbench
// and it'll have to rely on the macros for it to work
#include "cbench.h"

int add(int a, int b) { return a + b; }

int main(int argc, char *argv[]) {
    OBS_SETUP("TestingAdd", argc, argv);
    // a test group represents a selection of tests
    OBS_TEST_GROUP("Fuzzy Tests", {
        OBS_TEST("Basic", {
            obs_test_eq(int, add(5, 1), 6);
            obs_test_neq(int, add(10, 5), 15);
        })

        OBS_TEST("Pass", {
            obs_test_eq(int, add(10, -1), 9);
        })

        // purposefully wrong (10 + 2 != 15) just to show what happens when a test fails
        OBS_TEST("10 + 2", {
            // custom test assertion
            obs_test(add(10, 2) == 15, "add(%d, %d) = %d which is not 15", 10, 2, add(10, 2));
        })
    })

    OBS_BENCHMARK("Convert a bunch of stuff", 10, {
        for (double i = 0; i < 10000; i++) {
            printf("%lf %d %lf", i, (int)i, i * i);
        }
    })

    OBS_BENCHMARK("Find track parent", 10, {
        // should be 0, 0 but wall time won't be 0 since it's blocking
        system("find .");
    })

    OBS_BENCHMARK_CHILD("Find track children", 10, {
        system("find .");
    })

    OBS_BENCHMARK_SYS("Find using sys", 10, "find .")

    // And so on
    // report the results
    OBS_REPORT;
    return tests_failed;
}