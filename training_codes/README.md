## Organization

How does this repo is organized?

- `training_codes/`: this will be the main directories for gama training codes:
  - `training_codes/src/`: codes .cpp;
  - `training_codes/include/`: headers for all project codes;
  - `training_codes/build/`: artifacts added by `cmake`;
  - `training_codes/CMkaeLists.txt`: file for setting Makefile.

## Manual

### How to build

```bash
g++ <files_name.cpp> -o main `pkg-config --cflags --libs opencv4`
```

### How to run

```bash
./main
```
