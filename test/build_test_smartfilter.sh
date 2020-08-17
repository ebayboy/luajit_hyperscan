g++ -std=c++11 test_smartfilter.cpp -l smartfilter -L./ -o test_smartfilter
export LD_LIBRARY_PATH=:./:./
valgrind --tool=memcheck --leak-check=full ./test_smartfilter 
