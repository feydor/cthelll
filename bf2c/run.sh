#!/bin/bash
set -xe
cc -Wall -pedantic -g bf2c.c -o bf2c.out
./bf2c.out $1 > $1.IR.c
gcc -g $1.IR.c -o $1.c.out
