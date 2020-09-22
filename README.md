# calculator
Programs for the TI-84 Plus CE graphing calculator

## Programs

### Snake
![Snake](https://camo.githubusercontent.com/d329d9373fe7d331a53be20ae87b01ac35668370/68747470733a2f2f696d6775722e636f6d2f6c3167434b67632e676966)

## Installtion
Send the `.8xp` file as well as the [C libraries](https://github.com/CE-Programming/libraries/releases) over to your calculator using either TI Connect™ CE or TILP

## Building
Use the [CE-Programming's toolchain](https://github.com/CE-Programming/toolchain) to build the programs from source. I use LLVM to build and test, so there is a slight chance ZDS II may not compile.
In essence, make sure your `$CEDEV` environment variable is set correctly and run `make` in each program's respective directory.
