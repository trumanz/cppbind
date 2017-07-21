CC = g++
LD = $(CC)
CFLAGS =  -I./ 
CFLAGS =  -I./cppbind/include
CFLAGS += -c  -g -O0  -Wall 

LDFLAGS = -L../  -lcppbind
LDFLAGS += -lgtest  -lpthread 
LDFLAGS +=  -ljsoncpp 

TARGET = gtest.bin

SOURCES = $(shell find ./src -name "*.cpp")
OBJECTS= $(SOURCES:.cpp=.o)
##OBJECTS = $(patsubst %.c, %.o, $(shell find . -name "*.cpp"))

all: $(TARGET)

test: $(TARGET)
	./$(TARGET)
      

$(TARGET): $(OBJECTS)
	$(LD) -o $@ $^ $(LDFLAGS)

# You don't even need to be explicit here,
# compiling C files is handled automagically by Make.
%.o: %.cpp
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm $(TARGET) $(OBJECTS)


