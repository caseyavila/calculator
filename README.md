# calculator
Programs for the TI-84 Plus CE graphing calculator

## Programs

### Snake
![Snake](https://camo.githubusercontent.com/a8f674cfb77abe1115591c66d9e78c59d8757827a2303e76d760e641a6bfb842/68747470733a2f2f73342e61636f6e766572742e636f6d2f636f6e766572742f70337236382d63647836372f6133796c362d7663306c752e676966)

## Installtion
Send the `.8xp` file as well as the [C libraries](https://github.com/CE-Programming/libraries/releases) over to your calculator using either TI Connect™ CE or TILP

## Building
Use [CE-Programming's toolchain](https://github.com/CE-Programming/toolchain) to build the programs from source. I use LLVM to build and test, so there is a slight chance ZDS II may not compile.
In essence, make sure your `$CEDEV` environment variable is set correctly and run `make` in each program's respective directory.
