#!/bin/bash
set -xe
cc -Wall -pedantic -g bf2c.c -o bf2c.out
./bf2c.out $1 > $1.c
gcc -g $1.c -o $1.c.out
