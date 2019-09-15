# Customising Obsidian

Obsidian is very customisable this short guide will describe every aspect.

## Custom Assertions

Either build it up from multiple sub asserts or use `obs_test` i.e.

```c
struct vec {
  int x;
  int y;
  int z;
};

#define obs_test_vec_eq(a, b) \
  obs_test_eq(a.x, b.x); \
  obs_test_eq(a.y, b.y); \
  obs_test_eq(a.z, b.z); \

// ^^ you could also leave out the last semicolon to require a semicolon
// but personal pref.

// or you could just use obs_test

#define obs_test_vec_eq(a, b) \
  obs_test(a.x == b.x && a.y == b.y && a.z == b.z,\
           "(%d, %d, %d)" != (%d, %d, %d)", \
           a.x, a.y, a.z, b.x, b.y, b.z);

// you can of course write it using an if statement
#define obs_test_vec_eq(a, b) \
  if (a.x != b.x || a.y != b.y || a.z != b.z) { \
    obs_test(0, "(%d, %d, %d)" != (%d, %d, %d)", \
             a.x, a.y, a.z, b.x, b.y, b.z) \
  }
```

## Changing the string compare that is used

Just define `OBS_STRCMP`

```c
// important that it is before obsidian
#define OBS_STRCMP my_compare

#include "obsidian.h"
```

## Changing the memory compare that is used

Just define `OBS_MEMCMP`

```c
// important that it is before obsidian
#define OBS_MEMCMP my_compare

#include "obsidian.h"
```

## Don't Strip all arguments before '--'

You can disable stripping all arguments before '--' by definining OBS_DONT_STRIP_ARGS

```c
// important that it is before obsidian
#define OBS_DONT_STRIP_ARGS

#include "obsidian.h"
```

## Output changes

### Disabling the summary at the end

You can disable the summary at the end and any printing to the terminal via `OBS_NO_PRINT` this is probably necessary for embedded systems.  Files are always created for all the parts as usual.

### Colour

You can disable colour via `OBS_NO_COLOURS`

```c
// important that it is before obsidian
#define OBS_NO_COLOURS

#include "obsidian.h"
```

### Redirecting

Redirecting is where we redirect outputs to `/dev/null` (crossplatform'y it does work on windows using `NUL`)

You can disable redirections via `OBS_NO_REDIRECT` by default only stdout is redirected.

```c
// important that it is before obsidian
#define OBS_NO_REDIRECT

#include "obsidian.h"
```

If you want to redirect any files just call them like this

```c
#include "obsidian.h"

int main(int argc, char *argv[]) {
  OBS_SETUP("my project", argc, argv);

  FILE *f = fopen("my file", "w");
  OBS_REDIRECT_DEVNULL(my_file, f);

  // all the tests/benchmarks you want
  // ...

  // then you can restore it
  OBS_RESTORE(my_file, f);
  // and all writes will occur as you would expect

  // more tests/benchmarks if you want
  // ...

  OBS_REPORT
  return tests_failed;
}
```

You can cause it to redirect to something that isn't `/dev/null` (like a log file) using OBS_REDIRECT(id, out, to, mode);

```c
#include "obsidian.h"

int main(int argc, char *argv[]) {
  OBS_SETUP("my project", argc, argv);

  FILE *other_read = fopen("other file", "r");
  OBS_REDIRECT(stdin, stdin, other_read, "r");

  // all the tests/benchmarks you want
  // ...

  // then you can restore it
  OBS_RESTORE(stdin, stdin);
  // and all writes will occur as you would expect

  // more tests/benchmarks if you want
  // ...

  OBS_REPORT
  return tests_failed;
}
```

!> You probably want to disable redirects in embedded and handle the code yourself to redirect (since you often won't wont to print anything out that won't cause an issue).

### Formats

You can add extra formats by defining `OBS_EXTRA_FMT` this is for `obs_binop` i.e.

```c
// important that it is before obsidian
#define OBS_EXTRA_FMT \
  if (!strcmp(buf, "fakeint")) return "Evaulated: %d %s %d";

#include "obsidian.h"
```

Note it isn't very powerful since `obs_test` should have all the complex printing logic you require.  It is more meant for when you want to support extra typedefs of primatives.

It has to have the left argument be identical to the right with a string argument in the middle.  The string argument is for the negative operator.

The standard is to put `Evaulated:` prior to it.

!> There is a max type size of 256 by default you can override it by definining OBS_MAX_TYPE_SIZE with the size wanted

### Extra NegOps

By default it should support every operator but for whatever reason if you need to add extra operators they should work if the expression `a op b` is valid.

The only problem would be printing them this is where `OBS_EXTRA_NEG_OP` comes in similar to fmt you can define a negation operator format.

i.e.

```c
// important that it is before obsidian
#define OBS_EXTRA_NEG_OP \
  if (!strcmp(op, "??")) return "!!";

#include "obsidian.h"
```

## Benchmarking

### Disabling Benchmarks

You can disable all benchmarking by defining `OBS_NO_BENCHMARKS`

```c
// important that it is before obsidian
#define OBS_NO_BENCHMARKS

#include "obsidian.h"
```

This will disable all benchmarks by mapping them to just `;`, this means you can compile a test file with benchmarks using this #define to strip all the benchmarks out!  I prefer to just include an empty `-b` as a flag each time I don't want to run benchmarks but personal preference.

You can disable just children benchmarks (`OBS_BENCHMARK_CHILDREN` and `OBS_BENCHMARK_SYS`) via `OBS_NO_BENCHMARKS_CHILD`.

!> This macro is already defined on Windows (if using `cbench`) as currently `cbench` doesn't support getting the execution time of a child process.  So if you are using `cbench` on Windows and wondering why none of your child benchmarks are running then this is why.

### Changing how time 'getters'

Just define `OBS_GET_TIME`, `OBS_GET_WALL_TIME`, `obsTime`, and `OBS_GET_CHILDREN_TIME`.

You don't have to define `OBS_GET_CHILDREN_TIME` IF you define `OBS_NO_BENCHMARKS_CHILD`.

For example if I wanted to use cbench through the macros I would define;

```c
#define OBS_GET_TIME cbenchGetTime
#define OBS_GET_WALL_TIME cbenchGetWallTime
#define obsTime cbenchTime

// I don't have to define this I could also just
// define OBS_NO_BENCHMARKS_CHILD and just ignore all children
// benchmarks.
#define OBS_GET_CHILDREN_TIME cbenchGetChildrenTime

#include "obsidian.h"

// note how this is below obsidian which means it won't detect cbench
// and it'll have to rely on the macros for it to work
#include "cbench.h"
```

## Override the error signal raised

When using `-e` the app will raise an error signal if an error is reached.  You can change the signal used using `OBS_ERROR_SIGNAL` and `OBS_ERROR_SIGNAL_NAME` you have to define both!

```c
// important that it is before obsidian
#define OBS_ERROR_SIGNAL SIGINT
#define OBS_ERROR_SIGNAL_NAME "SIGINT"

#include "obsidian.h"
```
