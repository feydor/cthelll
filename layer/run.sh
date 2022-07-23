#!/bin/bash
set -xe
cc -Wall layer0.c -o 0l -lm
./0l $1
