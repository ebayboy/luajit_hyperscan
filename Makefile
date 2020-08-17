CC = g++
LD = g++

TARGET = build/libsmartfilter.so
INCLUDES = -I ./include 
LIB = -L ./lib
CFLAGS = -O0 -std=c++11 -Wall -fPIC
LDFLAGS = -shared -lhs

SRC := $(wildcard src/*.cpp) 
OBJS=$(SRC:.cpp=.o)

#$(warning  DEPS: $(DEPS))

.PHONY:all clean

all: $(TARGET)

$(TARGET): $(OBJS)
	$(LD) -o $@ $^ $(INCLUDES) $(LIB)  $(LDFLAGS)

%.o:%.cpp 
	$(CC) -c -g $< $(INCLUDES) $(CFLAGS) -o $@ 

clean:
	rm $(TARGET) $(OBJS) 
