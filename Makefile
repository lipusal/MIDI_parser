SOURCES=main.c MIDI.c
OBJECTS=main.o MIDI.o

all: $(SOURCES)
	make clean
	gcc $(SOURCES)	#Linux and Windows will use different GCC

clean:
	rm -rf *.o *.exe