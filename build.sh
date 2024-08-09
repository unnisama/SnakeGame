#!/bin/bash
clang++ main.cpp -lraylib -lGL -Ofast -o main

if [ $? == 0 ]; then
    ./main
fi