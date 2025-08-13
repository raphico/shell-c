[![progress-banner](https://backend.codecrafters.io/progress/shell/11d8ea3f-a41d-45c0-8e17-9e24e8530c94)](https://app.codecrafters.io/users/codecrafters-bot?r=2qF)

A minimal POSIX-compliant shell written in C that interprets commands, executes external programs, and supports built-in commands.

## Documentation

- [Project Motivation](./docs/motivation.md)

## Features implemented

1. Prompt display: Shows a prompt and waits for user input
2. Error handling: Gracefully handles invalid commands
3. REPL loop: Continuously reads, evaluates, and executes commands.
4. Built-in commands:
   - exit: Exit the shell
   - echo: Print text to stdout
   - type: Identify whether a command is a built-in or an executable
   - pwd: Print the current working directory
   - cd: Change directories: Absolute paths, Relative paths, Home directory (cd ~)
5. Quoting support:
   - Single quotes
   - Double quotes
   - Backslash escaping (inside and outside quotes)
   - Executing quoted executables
6. I/O Redirection:
   - Redirect stdout (>)
   - Redirect stderr (2>)
   - Append stdout (>>)
   - Append stderr (2>>)

## Features To Be Implemented

1. Tab completion:

   - Built-in completion
   - Executable completion
   - Multiple and partial completions

2. Pipelines:

   - Dual-command pipelines
   - Built-in commands in pipelines
   - Multi-command pipelines

3. Command history:
   - View history
   - Up/down arrow navigation
   - Execute commands from history
   - Persistent history (save/load from file)

## Tech Stack

- **Language:** C (C99 standard)
- **Standards Compliance:** POSIX
- **Platform:** Developed and tested on Linux. Compatible with macOS (POSIX-compliant). Windows support would require adapting process and file descriptor APIs to WinAPI.
- **Build System:** CMake (project configuration) with Ninja (fast incremental builds)
- **Compiler:** Clang (tested; GCC compatible)
- **Package Manager:** vcpkg for dependency management

## How to run

1. clone the repository

```bash
git clone git@github.com:raphico/shell-c.git
cd shell-c
```

2. run the program

```bash
./your_program.sh
```

## Project structure

```bash
├── include/              # Header files
├── src
│   ├── builtins/         # Built-in command implementations
│   ├── main.c            # Entry point: initializes and runs the shell REPL loop
│   ├── parser.c          # Tokenizes user input and parses redirection operators
│   ├── redirects.c       # Handles applying/restoring I/O redirections before/after command execution
│   └── shell.c           # Dispatches built-in commands or executes external programs
└── your_program.sh       # Shell script to run or test the program
```

## Attribution

This project was built as part of the [Codecrafters "Build Your Own Shell" Challenge](https://app.codecrafters.io/courses/shell/overview)
