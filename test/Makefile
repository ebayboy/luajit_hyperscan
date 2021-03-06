
CC := g++

SOURCE := $(wildcard *.c) 
SOURCE += $(wildcard *.cc) 
SOURCE += $(wildcard *.cpp)

# $(patsubst 原模式， 目标模式， 文件列表)
# $(patsubst %.c,%.o,$(dir) )中，patsubst 把$(dir)中的变量符合后缀是.c的全部替换成.o
# 匹配的部分替换，其他部分不变

OBJS   := $(patsubst %.c,%.o, $(patsubst %.cpp,%.o, $(patsubst %cc,%o, $(SOURCE)) ))
#$(warning "OBJS:"$(OBJS))

TARGET	:= test_static test_static_tcmalloc

DEFINES :=
LIBS	:= -L.
INCLUDE := -I.
LDFLAGS := -lpthread
CFLAGS	:= -c -std=c++17 -Wall

############## add self define  ######################

#DEFINES +=
#LIBS	+= 
INCLUDE += -I ../build/include/
LDFLAGS += ../build/lib/libsmartfilter.a
#CFLAGS	+= 

############## add self define  ######################

DEBUG=n
CONV=n

ifeq ($(DEBUG),y)
	CFLAGS += -g -O0
else
	CFLAGS += -O3 -g2
endif

ifeq ($(CONV),y)
	INCLUDE+= -I /usr/local/libiconv/include
	LIBS+= -L /usr/local/libiconv/lib

	CFLAGS += -fprofile-arcs -ftest-coverage
	LDFLAGS += -lgcov
endif

CXXFLAGS:= -DHAVE_CONFIG_H $(CFLAGS) $(DEFINES) $(INCLUDE)

.PHONY : everything objs clean veryclean rebuild

everything : $(TARGET)

all : $(TARGET)

objs : $(OBJS)

rebuild: veryclean everything

clean:
	rm -fr $(TARGET) $(OBJS)

veryclean : clean
	rm -rf *.so *.gcno *.gcda

%.o:%.c
	$(CC) $(CXXFLAGS) $< $(INCLUDE) -o $@

%.o:%.cc
	$(CC) $(CXXFLAGS) $< $(INCLUDE) -o $@

%.o:%.cpp
	$(CC) $(CXXFLAGS) $< $(INCLUDE) -o $@

test_static : $(OBJS)
	$(CC) -o $@ $(OBJS) $(LDFLAGS) $(LIBS)

test_static_tcmalloc : $(objs)
	$(CC) -o $@ $(OBJS) $(LDFLAGS) $(LIBS) /usr/local/gperftools/lib/libtcmalloc_minimal.a

