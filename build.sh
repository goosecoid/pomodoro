#! /bin/sh
clang -framework CoreVideo -framework IOKit -framework Cocoa -framework GLUT -framework OpenGL libraylib.a main.c -o main -Wall -Wextra -O2 -std=c99 -pedantic
