CC=gcc
CPP=g++
CFLAGS= -c -Wall
CPPFLAGS= -c -Wall -I/if10/lw2aw/boost/include 
LDFLAGS= -static -L/if10/lw2aw/boost/lib
LIBS= -lboost_graph
SOURCES= heapsort.cpp main.cpp
OBJECTS= $(SOURCES:.cpp=.o)
HEADER= heapsort.h
TARGET= heapsort

all: $(SOURCES) $(TARGET)

$(TARGET): $(OBJECTS)
	$(CPP) $(LDFLAGS) $(OBJECTS) -o $@ $(LIBS)

%.o: %.cpp %.h
	$(CPP) $(CPPFLAGS) $< -o $@

clean:
	rm *.o $(TARGET)
