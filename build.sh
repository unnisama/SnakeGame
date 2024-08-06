#!/bin/bash
clang++ main.cpp -lraylib -lGL -o main

if [ $? == 0 ]; then
    ./main
fi