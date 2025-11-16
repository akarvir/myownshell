# myownshell

I built a tiny,Unix-like shell written in C.

This repository contains a simple shell implementation intended for learning and experimentation.

## Files

- `shell.c` — main source for the shell implementation

## Build

Build with GCC (macOS / Linux):

```bash
gcc -Wall -Wextra -o shell shell.c
```

If `trial.c` is part of your intended build, compile both:

```bash
gcc -Wall -Wextra -o shell shell.c trial.c
```

## Run

Start the shell from the project directory:

```bash
./shell
```

Type commands and press Enter. To exit, use `exit` or press `Ctrl-D`.

