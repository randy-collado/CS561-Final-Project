# Concurrency-Aware Graph/Tree-traversal Algorithms on SSD

This is a project to utilize internal concurrency of SSD to accelerate *no-cache* I/O in searching/traversing graphs and trees. It has been tested on Windows 10/11 and Linux.

## Build

Goto ./cpp folder, then simply run

```bash
make
```

Note: If you build this on Windows, make sure you are *not* using MinGW GCC 12.1, which has a known bug in OpenMP implementation. See here: https://github.com/msys2/MINGW-packages/issues/11729.

## Usage

```bash
# Generate test data
./generator <data_path> <num_elements> <branch>

# Run on generated data
./main <data_path>
```