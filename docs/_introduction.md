# Obsidian - C unit testing and benchmarking framework

A very simple C unit testing and benchmarking framework that is extremely customizable

## Features

- Should work on embedded (with some #defines set)
- Flags done automatically and you can even strip out the flags to allow your application to seamlessly use argc/argv
- Nice syntax for assertions
- Custom assertion support is easy
- Can benchmark system and child processes as well.

## Installing

Just download the `obsidian.h` file and if you want to do benchmarking you'll also need `cbench.h` from [here](https://github.com/BraedonWooding/cbench).

?> You can use a custom benchmarking library see [Customisations](_customisations#benchmarking)

## Unit Testing Example

?> Note in all the outputs the dots are also coloured to indicate which tests pass/fail but I can't show this through the highlighting

Whenever you compile and run a file will be created for each segment: benchmarks, errors, and just the general log.  These will end with `.log`.

<!-- tabs:start -->

#### ** Code **

```c
#include "obsidian.h"

int add(int a, int b) { return a + b; }

int main(int argc, char *argv[]) {
    OBS_SETUP("TestingAdd", argc, argv);
    // a test group represents a selection of tests
    OBS_TEST_GROUP("Fuzzy Tests", {
        OBS_TEST("Basic", {
            obs_test_eq(int, add(5, 1), 6);
            // wrong to show the error msg
            obs_test_neq(int, add(10, 5), 15);
        })

        OBS_TEST("Pass", {
            obs_test_eq(int, add(10, -1), 9);
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

#### ** Output **

```output
Running Fuzzy Tests ... failed (2/3 tests failed)
Log Files are at: TestingAdd.log and TestingAdd_err.log

==Errors==
example_test.c:11 TEST FAILED: Fuzzy Tests:Basic
Assert (add(10, 5) != 15)
Evaluated: 15 == 15

example_test.c:17 TEST FAILED: Fuzzy Tests:10 + 2
Assert add(10, 2) == 15
add(10, 2) = 12 which is not 15
```

<!-- tabs:end -->
## Benchmarks (Just current process)
Use cbench or your own benchmarking library check out customisation for ways to add your own.

To use cbench just download `cbench.h` file from [here](https://github.com/BraedonWooding/cbench)

!> Currently only supported on posix platforms.  Until I can find a nice way to do it on windows.

<!-- tabs:start -->

#### ** Code **

```c
// you have to include your benchmarking library before obsidian
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

    // all benchmarks just track parents by default
    OBS_BENCHMARK("Find track parent", 10, {
        // should be 0, 0 but wall time won't be 0 since it's blocking
        system("find .");
    })

    // this will track all children (but not the parent)
    OBS_BENCHMARK_CHILD("Find track children", 10, {
        system("find .");
    })

    // this runs an external 'system' command and tracks the time
    OBS_BENCHMARK_SYS("Find using sys", 10, "find .")

    OBS_REPORT
    return tests_failed;
}
```

#### ** Raw Results **

```output
Benchmark log file is at: TestingAdd_benchmark.log

==Benchmarks==
Benchmarking Convert a bunch of stuff (10 time/s)
| usr (s)        | sys (s)        | wall (s)       |
| -------------- | -------------- | -------------- |
| 0.007 +- 0.002 | 0.000 +- 0.001 | 0.007 +- 0.004 |

Benchmarking Find track parent (10 time/s)
| usr (s)        | sys (s)        | wall (s)       |
| -------------- | -------------- | -------------- |
| 0.000 +- 0.000 | 0.000 +- 0.000 | 0.008 +- 0.000 |

Benchmarking Find track children (10 time/s)
| usr (s)        | sys (s)        | wall (s)       |
| -------------- | -------------- | -------------- |
| 0.003 +- 0.000 | 0.005 +- 0.001 | 0.009 +- 0.001 |

Benchmarking Find using sys (10 time/s)
| usr (s)        | sys (s)        | wall (s)       |
| -------------- | -------------- | -------------- |
| 0.003 +- 0.000 | 0.004 +- 0.000 | 0.008 +- 0.000 |
```

#### ** Analysis of Results **

Benchmarking Convert a bunch of stuff (10 times/s)

| usr (s)       | sys (s)       | wall (s)      |
| ------------- | ------------- | ------------- |
| 0.007 ± 0.001 | 0.000 ± 0.000 | 0.008 ± 0.001 |

?> System is 0 because stdout is piped to /dev/null by default you can either disable this or just write it to a file that you remove.

Benchmarking Find track parent (10 time/s)

| usr (s)       | sys (s)       | wall (s)      |
| ------------- | ------------- | ------------- |
| 0.000 ± 0.000 | 0.000 ± 0.000 | 0.009 ± 0.003 |

?> Note how this is 0, 0 because the parent is sleeping the entire time

Benchmarking Find track children (10 time/s)

| usr (s)       | sys (s)       | wall (s)      |
| ------------- | ------------- | ------------- |
| 0.003 ± 0.000 | 0.005 ± 0.001 | 0.008 ± 0.001 |

?> By using the children form we track the children command time (note also that the wall time is approx the same as we would expect)

Benchmarking Find using sys (10 time/s)

| usr (s)       | sys (s)       | wall (s)      |
| ------------- | ------------- | ------------- |
| 0.003 ± 0.000 | 0.004 ± 0.000 | 0.008 ± 0.000 |

?> Basically the same results as above as one would expect

<!-- tabs:end -->

## All assertions available

?> It is easy to create your own by defining a macro that uses `obs_test`!

!> `obs_test_binop`, and all the comparison ones will evaulate all their args twice make sure you cache anything important

- `obs_test(cond, fmt, args...)`
  - i.e. `obs_test(a > 0, "a (%d) is positive", a);`
- `obs_test_binop(type, x, op, y)`
  - i.e. `obs_test_binop(int, x, ==, y);`
- `obs_test_eq(type, x, y)` equivalent to `obs_test_binop(type, x, ==, y)`
- `obs_test_neq(type, x, y)` equivalent to `obs_test_binop(type, x, !=, y)`
- `obs_test_lt(type, x, y)` equivalent to `obs_test_binop(type, x, <, y)`
- `obs_test_gt(type, x, y)` equivalent to `obs_test_binop(type, x, >, y)`
- `obs_test_lte(type, x, y)` equivalent to `obs_test_binop(type, x, <=, y)`
- `obs_test_gte(type, x, y)` equivalent to `obs_test_binop(type, x, >=, y)`
- `obs_test_str_eq(x, y)`
  - Does a strcmp, you can override the strcmp done via defining `OBS_STRCMP` as talked about in [Customisations](_customisations)
- `obs_test_str_neq(x, y)` boolean negation of above.
- `obs_test_null(x)` => `obs_test_eq(void*, x, NULL)` with nicer output
- `obs_test_not_null(x)` => `obs_test_neq(void*, x, NULL)` with nicer output
- `obs_test_true(x)` => `obs_test_eq(bool, x, true)` with nicer output
- `obs_test_false(x)` => `obs_test_eq(bool, x, false)` with nicer output

## Flags

You can pass arguments to your program by passing them after a `--` i.e.

```bash
./a.out -b "MyBenchmark" -v -e -- my own flags are here 20
```

Your program would see the following

```bash
./a.out my own flags are here 20
```

### Obsidian Flags

Obsidian also comes with a series of flags with all compiled programs they are;

- `-b` or `--benchmarks` enable only the tests that follow this command i.e. `-b Hello "Use quotes for things with spaces" other` will only run `Hello`, `Use quotes for things with spaces` and `other`.
  - i.e. if you want to run no benchmarks do `-b` by itself
- `-t` or `--tests` is exact same as `-b` but for tests, for example `-t 1 Simple "1 + 2"` will run `1`, `Simple`, `1 + 2`
  - i.e. no tests is going to be `-t`
- `-g` or `-test-groups` is exact same as `-t` and `-b` but for test groups, for example `-g Invalid "Over the network"` will run only the groups `Invalid`, `Over the network`.
- `-e` or `--raise-on-error` will raise a signal on the first error (SIGABRT by default you can change that though) you can then attach yourself to this error using your favourite debugger.
  - Tips are at [Debugging Tips](_debugging)
- `-v` print out version information
