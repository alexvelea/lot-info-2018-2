#!/bin/bash

NAME=turneu

gcc -static -O2 -std=gnu11 -c -o $NAME.o $NAME.c -lm 2>&1
g++ -O2 -std=c++11 -static grader.cpp $NAME.o -o the.exe -lm 2>&1
