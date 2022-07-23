#!/bin/bash
set -xe
cc -Wall layer0.c -o layer0 -lm
./0layer $1
