TARGET = ./src/libguisan.a
CFLAGS = -I include -D_THREAD_SAFE
CPPFLAGS = -I include -D_THREAD_SAFE
HEADER = include/guisan.hpp
AR = ar
SUBDIRS=$(shell find . -type d|grep -v demo|grep -v calculator|grep -v examples|grep src)
SRCS=$(foreach dir, $(SUBDIRS), $(wildcard $(dir)/*.c $(dir)/*.cpp)) 
OBJS = $(SRCS:.c=.o)
OBJS = $(SRCS:.cpp=.o)

$(TARGET):$(OBJS)
	$(AR) -rc $(TARGET) $(OBJS)
install:
	cp -r $(HEADER) /usr/local/include
	cp $(TARGET) /usr/local/lib
run:
	./calculator/calculator
all:
	make
	cd calculator && make
unistall:
	cd /usr/local && rm -rf $(HEADER)
	cd /usr/local/lib && rm -rf libguisan.a
