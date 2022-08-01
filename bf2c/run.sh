#!/bin/bash
set -xe
cc -Wall -pedantic -g bf2c.c -o bf2c.out
./bf2c.out $1
