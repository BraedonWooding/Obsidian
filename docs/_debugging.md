# Debugging

- Compile with your associated debug settings (i.e. `-gdwarf` for mac, ...)
- Then open with your favourite debugger and get it to stop on any signals (you can specify it, by default its SIGABRT and you can edit that).
- You can then just inspect all variables as you would
- For `gdb` / `lldb`
  - These are pretty basic tips but should help if you aren't very comfortable with these tools
  - Use `bt` to find all stack frames identify the `main` stackframe
  - Pick the main stack frame using `f` i.e. if you main stackframe is #3 then you can do `f 3`
  - `print` (or `p`) to print out variables
  - To see all variables in the frame you can use `fr v` (or `frame var`)

!> Obsidian doesn't handle SEGFAULTS or signals raised by the program it will just crash as usual this was due to the complexity of sandboxing it.  If you want to debug these it is just as easy as usual just attach your debugger and stop on the signals and inspect frames.
