# esh — Easy Shell for Windows

**esh (Easy Shell)** is an experimental command-line shell written in modern C++
for Windows. 

The project focuses on understanding shell internals, command
execution flow, and Windows API–based system programming rather than providing a
fully featured production shell.

esh is a completed learning-oriented systems programming project.

---

## Goals

- Understand how a shell parses and executes commands
- Learn Windows API fundamentals relevant to process and file management
- Design a clean internal command/flag/symbol architecture
- Gain experience with Unicode handling (UTF-8 / UTF-16) on Windows
- Build a non-trivial C++ project with clear structure and layering


## Features

- Built-in commands (file, process, system, and shell commands)
- Command grouping and internal command registry
- Flag-based command behavior
- Pipeline and redirection parsing
- JSON-based help system
- Unicode-safe input and output
- Colored console output


## Project Structure

```
esh/
├─ src/
│  ├─ core/        # Core shell engine (parser, lexer, execution flow)
│  ├─ commands/    # Built-in command implementations
│  ├─ execution/   # Command execution and process control
│  ├─ file/        # File-related commands and utilities
│  ├─ process/     # Process-related commands
│  ├─ env/         # Environment-related commands
│  ├─ shell/       # Shell-specific commands and logic
│  ├─ system/      # System information and system commands
│  ├─ history/     # Command history management
│  ├─ platform/    # Platform-specific utilities (Windows)
│  ├─ headers/     # Public headers and shared interfaces
│  └─ external/    # Third-party headers (e.g. JSON)
│
├─ resources.rc # Embeds json file to .exe
├─ app.rc # Windows resource file
├─ icon/esh.ico        # Application icon
│
├─ build/          # Build output (not tracked)
├─ esh.json        # Command help and metadata
├─ CMakeLists.txt  # CMake configuration
├─ Makefile        # Build entry point
├─ README.md
└─ LICENSE
```

## Build Information

- **Language:** C++ (modern standard)
- **Compiler:** LLVM Clang (Windows)
- **Platform:** Windows (Win32 API)
- **Encoding:** UTF-16 internally, UTF-8 at boundaries
- **Build system:** CMake and Makefile


## Requirements

To build and run **esh**, you need:

- **Windows** (Win32 API required)
- **LLVM Clang (clang-cl)** (as the C/C++ compiler)
- **CMake**
- **Make**
- A Windows environment capable of building Win32 applications

esh relies directly on the Windows API and is not portable to other platforms.


## Build & Run

From the project root directory:

```sh
make # make configures and generates build file of the project using CMake and Clang
make run # compiles the project and generates esh.exe
```

## Project Status

This project is **feature-complete** and **not under active development**.
The repository is preserved as a reference and learning artifact.


## Versioning

Current version: **0.1.0**


## License

Licensed under the **Apache License 2.0**.  
See the `LICENSE` file for details.


## Disclaimer

esh is an educational and experimental project.
It is not intended for production use.
You may encounter bugs while working with esh.

---

## Author

Habil Eren Türker
