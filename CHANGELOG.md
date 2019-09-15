# Change Log

## 1.1a

- No longer relies on GNU for var arg counts of 0
  - The behaviour should be the same we just made it so that the var arg sucks up the format string as well this should not change how you use the function but makes it more portable.
- No longer relies on initialisations in for loops (i.e. c99 for loops)
  - This means we are actually C89 compilant
  - Let me know if I missed anything else
- `obs_test_mem_eq(test, x, y)` and `obs_test_mem_neq(test, x, y)` that utilise memcmp added
  - You can override the memcmp used of course
- No longer relies on GNU for named var args
  - Does hurt readability a touch (but not considerably) but was necessary to make it more portable.
  - No functionality impact
- `obs_err(...)` added for when you really want to bring the error outside of the testing condition.
  - i.e. `if (a != b) { obs_err("ASSERT: a != b\n%EVALUATION: d != %d", a, b); }` is effectively the same as `obs_test_eql` (roughly same output could make it the same if wanted though).
  - I wasn't sure about adding this but in the end it felt right when I was using it in WhyJson in some cases.

## 1.0c

- Some small reformatting since I felt I might as well do it now, I formatted it to a 'google' standard
- Bug fix around naming with redirect devnull
  - Had to do a weird ish workaround to avoid the id from being expanded literally i.e. to keep id stdout as stdout and not the expansion of it
- Removed variables that weren't used

## 1.0b


- Docs complete
- Version printing now works.

## 1.0a

- 'Released'
- Added support for benchmarking
- Added some examples
