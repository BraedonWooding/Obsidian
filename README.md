# Obsidian - C testing framework

Just a testing framework built for C.

TODO: Much better documentation (sorry I've been pretty busy with CPath, I'll come back to this eventually).

## Example

```c
#include "obsidian.h"

int add(int a, int b) { return a + b; }

int main(int argc, char *argv[]) {
    OBS_SETUP("TestingAdd", argv, argc);
    // a test group represents a selection of tests
    OBS_TEST_GROUP("Fuzzy Tests", {
        OBS_TEST("Basic", {
            obs_test_eql(int, add(5, 1), 6);
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
```

## Benchmarks

Use cbench or your own benchmarking library;

```c
#include "cbench.h"
#include "obsidian.h"
#include <string.h>

int main(int argc, char *argv[]) {
    OBS_SETUP("Benchmarks", argc, argv);
    OBS_BENCHMARK("Convert a bunch of stuff", 10, {
        for (double i = 0; i < 10000; i++) {
            printf("%lf %d %lf", i, (int)i, i * i);
        }
    })
    
    return tests_failed;
}
```

Results on my system:

```bash
Benchmarking Convert a bunch of stuff (10 times/s)
| usr (s)        | sys (s)        | wall (s)       |
| -------------- | -------------- | -------------- |
| 0.007 +- 0.001 | 0.000 +- 0.000 | 0.008 +- 0.001 |
```

System is 0 because stdout is piped to /dev/null by default you can either disable this or just write it to a file that you remove.

You can also use `OBS_BENCHMARK_SYS` to benchmark an external command (uses system) and `OBS_BENCHMARK_CHILDREN` to benchmark all children processes.
