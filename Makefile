CC = g++
LD = g++
AR		:= ar
RANLIB	:= ranlib

TARGET = build/lib/libsmartfilter.so
TARGET_STATIC = .libsmartfilter.a

INCLUDES := -I ./include 
CFLAGS := -std=c++17 -Wall -fPIC
LDFLAGS := ./lib/libhs.a 
LIB := 

SRC := $(wildcard src/*.cpp) 
OBJS=$(SRC:.cpp=.o)

DEBUG=n
CONV=n

ifeq ($(DEBUG),y)
	CFLAGS += -g2 -O0
else
	CFLAGS += -O2
endif

#$(warning  DEPS: $(DEPS))

.PHONY:all clean

all: $(TARGET) $(TARGET_STATIC)

$(TARGET): $(OBJS)
	mkdir -p ./build/{include,lib,bin} 2>&1 &>/dev/null
	$(LD) -o $@ $^ $(INCLUDES) $(LIB)  $(LDFLAGS) -shared
	cp -af src/smartfilter.h build/include/

$(TARGET_STATIC) : $(OBJS) 
	$(AR) cru $(TARGET_STATIC) $(OBJS) 
	$(RANLIB) $(TARGET_STATIC)
	./merge_libs.sh

%.o:%.cpp 
	$(CC) -c $< $(INCLUDES) $(CFLAGS) -o $@ 

clean:
	rm -rf ./build/* $(TARGET_STATIC) $(TARGET) $(OBJS)  ./tmp
