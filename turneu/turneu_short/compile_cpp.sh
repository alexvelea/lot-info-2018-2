#!/bin/bash

NAME=turneu

g++ -std=c++11 -O2 -static -o the.exe grader.cpp $NAME.cpp -lm 2>&1
