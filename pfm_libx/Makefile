CC=gcc
AR=ar
CFLAGS=-Wall -I/if14/ww6r/Public/libpfm-4.2.0/include/ -I/if14/ww6r/Public/common_toolx/
LDFLAGS= 
LIBS=
SOURCES=pfm_libx.c perf_util.c
OBJECTS=$(SOURCES:.c=.o)
SLIB=libpfmlibx.a

all: $(SOURCES) $(SLIB)

$(SLIB): $(OBJECTS)
	$(AR) rcs $(SLIB) $(OBJECTS)

.c.o:
	$(CC) -c $(CFLAGS) $< -o $@

clean:
	rm *.o $(SLIB)