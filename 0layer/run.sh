#!/bin/bash
set -xe
cc -Wall -g layer0.c -o 0layer.out -lm
./0layer.out $1
rm 0layer.out
