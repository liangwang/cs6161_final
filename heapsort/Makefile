CC=gcc
CPP=g++
CFLAGS= -Wall -O3
CPPFLAGS= -Wall -I/if10/lw2aw/boost/include -I../pfm_libx/ -O3
LDFLAGS= -static -L/if10/lw2aw/boost/lib  -L../pfm_libx/ -L/if14/ww6r/Public/libpfm-4.2.0/lib
LIBS= -lboost_graph -lpfmlibx -lpfm
SOURCES= heapsort.cpp main.cpp
OBJECTS= $(SOURCES:.cpp=.o)
HEADER= heapsort.h
TARGET= heapsort

all: $(SOURCES) $(TARGET)

$(TARGET): $(OBJECTS)
	$(CPP) $(LDFLAGS) $(OBJECTS) -o $@ $(LIBS)

%.o: %.cpp %.h
	$(CPP) $(CPPFLAGS) -c $< -o $@

clean:
	rm *.o $(TARGET)
datagen:
	$(CPP) $(CFLAGS) data_gen.c -o data_gen
