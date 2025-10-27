## Organization

How does this repo is organized?

- `training_codes/`: this will be the main directories for gama training codes:
  - `training_codes/build/`: artifacts added by `cmake`;
  - `training_codes/images/`: all images used by src files;
  - `training_codes/results`: have all results from src files
  - `training_codes/scripts`: all scripts to run `cmake`;
  - `training_codes/src/`: codes .cpp;
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
