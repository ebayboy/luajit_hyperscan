g++ -std=c++11 test_smartfilter.cpp -l smartfilter -L./ -o test_smartfilter
export LD_LIBRARY_PATH=:./:./
./test_smartfilter 
