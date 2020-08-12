CC = g++
LD = g++

TARGET = liblhps
INCLUDES = -I../include -I./
LIB = -L../libs 
CFLAGS = -std=c++11 -Wall  -lhs -fPIC -shared

SRC := $(wildcard src/*.cpp) 
OBJS=$(SRC:.cpp=.o)

#$(warning  DEPS: $(DEPS))

.PHONY:all clean

all: $(TARGET)

$(TARGET): $(OBJS)
	$(LD) -o $@ $^ $(INCLUDES) $(LIB) 

%.o:%.cpp 
	$(CC) -c -g $< $(INCLUDES) $(CFLAGS) -o $@ 

clean:
	rm $(TARGET) $(OBJS) 
