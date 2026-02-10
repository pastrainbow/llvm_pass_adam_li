# Task 2, LLVM Pass - Adam Li

## Requirements

The library is built with:

* **LLVM version:** 18.1.3
* **Clang version:** 18.1.3
* **CMake version:** 3.28.3

---

## Build Instructions

### 1. Compile C Program to LLVM Bitcode (assuming your program is called program.c)

```bash
clang -c -emit-llvm program.c
```

This generates:

```
program.bc
```

---

### 2. Build the LLVM Pass Plugin

```bash
cmake -S . -B build -DLLVM_DIR="$(llvm-config --cmakedir)"
cmake --build build
```
The first command configures the build, specifyinig the source directory,
the build directory, and the location of LLVM’s CMake config files. It will
generate the CMake artifacts in the build directory.

The second command runs the generated build tool in the build directory,
producing the library file:

```
CountFnBBPass.so
```

---

## Running the Pass

Execute the library using `opt`:

```bash
opt -load-pass-plugin=./CountFnBBPass.so -passes="count-fn-bb-pass" program.bc -o /dev/null
```

This loads the pass plugin and runs `count-fn-bb-pass` on `program.bc`.

---
