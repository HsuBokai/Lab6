CXX = g++
#CFLAGS = -g -Wall
CFLAGS = -O3

LIBPKGS = util
INCLIB = $(addprefix -l,$(LIBPKGS) )
LIBDIR = util
EXTINCDIR = util

EXEC = bin/lab6
COBJS = main.o
#COBJS = $(addsuffix .o, $(basename $(wildcard *.cpp) ) )

all: libs $(EXEC)

libs:
	cd $(LIBPKGS); make --no-print-directory; cd ..;

$(EXEC): $(COBJS)
	$(CXX) $(CFLAGS) -I$(EXTINCDIR) $? -L$(LIBDIR) $(INCLIB) -o $@; \
	rm $?

%.o: %.cpp
	$(CXX) $(CFLAGS) -I$(EXTINCDIR) -c -o $@ $<

clean:
	rm $(EXEC)
	cd $(LIBPKGS); make --no-print-directory clean; cd ..;
	
