#!/bin/bash

#g++ -std=c++11 simplegrep.c ../lib/libhs.a -I ../include/ -o simplegrep

g++ -O3 -std=c++17 simplegrep_multi.c ../lib/libhs.a -I ../include/ -o simplegrep_multi /usr/local/gperftools/lib/libtcmalloc_minimal.a
