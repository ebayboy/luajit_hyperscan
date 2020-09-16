CC = g++
LD = g++

TARGET = test

INCLUDES := -I /usr/local/googletest/include/ -I /usr/local/glog/include/
LIB = 
CFLAGS := -c -g -O0 -std=c++17 -Wall
LDFLAGS := -lpthread 
LDFLAGS += /usr/local/googletest/lib64/libgtest.a

SRC_C := $(wildcard *.c)
SRC_CPP := $(wildcard *.cc)

OBJS_C=$(SRC_C:.c=.o)
OBJS_CPP=$(SRC_CPP:.cc=.o)

#===========add other libs start
INCLUDES += -I ../
INCLUDES += -I /usr/local/hyperscan/include/
LDFLAGS += /usr/local/hyperscan/lib/libhs.a
#===========add other libs end

#$(warning $(OBJS_C))

.PHONY:all clean

all: $(TARGET)
$(TARGET): $(OBJS_C) $(OBJS_CPP)
	$(LD) $^ -o $@ $(LDFLAGS) $(LIB) 

%.o:%.c
	$(CC) $(CFLAGS) $< $(INCLUDES) -o $@

%.o:%.cc
	$(CC) $(CFLAGS) $< $(INCLUDES) -o $@

clean:
	rm -rf $(TARGET) $(OBJS_C) $(OBJS_CPP)

