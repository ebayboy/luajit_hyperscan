#!/bin/bash

## build shared demo
#g++ -O3 -std=c++17 test_smartfilter.cpp -l smartfilter -L./ -o test_smartfilter
#export LD_LIBRARY_PATH=:./:./
#valgrind --tool=memcheck --leak-check=full ./test_smartfilter 

## build static demo
g++ -std=c++17 -O3 -o test_smartfilter_static test_smartfilter.cpp ../build/lib/libsmartfilter.a -I ../build/include/ ../lib/libhs.a 
