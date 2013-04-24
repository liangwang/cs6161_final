CC=gcc
CPP=g++
CFLAGS=-Wall -O3
CPPFLAGS=-Wall -I/if10/lw2aw/boost/include -O3
LDFLAGS=-L/if10/lw2aw/boost/lib -O3
LIBS=
SOURCES= dijkstra_main.cc graph.cc
OBJECTS= $(SOURCES:.cc=.o)
HEADER= graph.hh
TARGET= dijkstra

all: $(SOURCES) $(TARGET)

$(TARGET): $(OBJECTS)
	$(CPP) $(LDFLAGS) $(OBJECTS) -o $@ $(LIBS)

%.o: %.cc %.hh
	$(CPP) $(CPPFLAGS) -c $< -o $@

clean:
	/bin/rm -f *.o $(TARGET)
