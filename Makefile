CC = g++
LD = $(CC)
CFLAGS =  -I./ 
CFLAGS =  -I./cppbind/include
CFLAGS += -c  -g -O0  -Wall 
CFLAGS +=  -fPIC

LDFLAGS = -L./ 
LDFLAGS = -lgtest  -lpthread 
LDFLAGS +=  -ljsoncpp 
LDFLAGS +=  -shared

TARGET = libcppbind.so

SOURCES = $(shell find ./cppbind -name "*.cpp")
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


