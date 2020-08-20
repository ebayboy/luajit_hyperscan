#!/bin/bash

## build shared demo
#g++ -O3 -std=c++17 test_smartfilter.cpp -l smartfilter -L./ -o test_smartfilter
#export LD_LIBRARY_PATH=:./:./
#valgrind --tool=memcheck --leak-check=full ./test_smartfilter 

## build static demo
g++ -std=c++17 -O3 -o test_static test_smartfilter.cpp ../build/lib/libsmartfilter.a -I ../build/include/ ../lib/libhs.a 


## build with tcmalloc
g++ -std=c++17 -O3 -o test_static_tcmalloc test_smartfilter.cpp ../build/lib/libsmartfilter.a -I ../build/include/ ../lib/libhs.a  -ltcmalloc -L /usr/local/tcmalloc/lib

##build tc_static
g++ -std=c++17 -O3 -o test_static_tcmalloc_static test_smartfilter.cpp ../build/lib/libsmartfilter.a -I ../build/include/ ../lib/libhs.a  /usr/local/tcmalloc/lib/libtcmalloc.a -lpthread
