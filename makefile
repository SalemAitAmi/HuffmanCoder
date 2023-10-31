#CC=gcc                                # Use this to compile on your own computer (confirmed to work on WSL)
#CC=/opt/arm/4.3.2/bin/arm-linux-gcc   # Use this to compile on the arm intranet
#CFLAGS=-Wall -g -O3           		   # Use this to enable debugger flags in order to perform cache analysis using valgrind --tool=cachegrind ./HuffmanEncoderV2 sample#.txt
#CFLAGS=-Wall -pg -O3                  # Use this in order to enable gprof for profiling 
#CFLAGS=-Wall -O3                      # Use this to compile normally with full compiller optimizations
#CFLAGS=-Wall -O0                      # Use this to compile normally without compiller optimizations
HEADERS=HuffmanHeader.h
OBJECT=HuffmanEncoderV2.o

all: HuffmanEncoderV2

HuffmanEncoderV2: $(OBJECT)
	$(CC) -o HuffmanEncoderV2 $(OBJECT) $(CFLAGS)
	-rm -f $(OBJECT)

HuffmanEncoderV2.o: HuffmanEncoderV2.c
	$(CC) $(CFLAGS) -c HuffmanEncoderV2.c

clean:
	-rm -f $(OBJECT) HuffmanEncoderV2
