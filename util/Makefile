CC = g++
CFLAGS = -g -Wall
AR = ar
ARFLAGS = -cr

OBJ := $(patsubst %.cpp, %.o, $(wildcard *.cpp))
EXTINCDIR = .
TARGET = libutil.a

all: $(TARGET)

$(TARGET): $(OBJ)
	$(AR) $(ARFLAGS) $@ $?

%.o: %.cpp
	$(CC) $(CFLAGS) -I$(EXTINCDIR) -c -o $@ $<

clean:
	rm $(OBJ) $(TARGET)
