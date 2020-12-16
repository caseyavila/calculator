# calculator
Programs for the TI-84 Plus CE graphing calculator

## Programs

### Snake
![Snake](https://i.imgur.com/9bsFf32.gif)

## Installtion
Send the `.8xp` file as well as the [C libraries](https://github.com/CE-Programming/libraries/releases) over to your calculator using either TI Connect™ CE or TILP

## Building
Use [CE-Programming's toolchain](https://github.com/CE-Programming/toolchain) to build the programs from source. LLVM is used to build and test, so there is a slight chance ZDS II may not compile.
In essence, make sure your `$CEDEV` environment variable is set correctly and run `make` in each program's respective directory.
