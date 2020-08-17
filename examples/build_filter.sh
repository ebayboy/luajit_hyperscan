#!/bin/bash

g++ -O0 -c -fPIC smartfilter.cpp -o smartfilter.o -I ../include/
g++ -shared smartfilter.o ../lib/libhs.a -o libsmartfilter.so -I ../include/
