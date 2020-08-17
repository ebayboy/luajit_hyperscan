#!/bin/bash

g++ -std=c++11 simplegrep.c -lhs -L ../lib/ -I ../include/ -o simplegrep
