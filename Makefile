CC = g++
LD = $(CC)
CFLAGS = -c  -g -O0 -I./  -Wall
CFLAGS += -fopenmp

LDFLAGS = -L./  -lgtest  -lpthread 
LDFLAGS +=  -lmordor
LDFLAGS +=  -lboost_system -lboost_thread  -lboost_regex 
LDFLAGS +=  -lcrypto 
LDFLAGS +=  -lsqlite3 
LDFLAGS +=  -ljsoncpp
LDFLAGS +=  -lthrift
LDFLAGS += -fopenmp


TARGET = a.out

SOURCES=$(shell find . -name "*.cpp")
OBJECTS= $(SOURCES:.cpp=.o)
##OBJECTS = $(patsubst %.c, %.o, $(shell find . -name "*.cpp"))

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(LD) -o $@ $^ $(LDFLAGS)

# You don't even need to be explicit here,
# compiling C files is handled automagically by Make.
%.o: %.cpp
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm $(TARGET) $(OBJECTS)

test: all
	./$(TARGET)
