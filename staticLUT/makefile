CC=gcc
#CFLAGS=-Wall -Wextra -g -O3  # Use this to perform cache analysis
#CFLAGS=-Wall -Wextra -pg -O3 # Use this to use gprof for profiling 
CFLAGS=-Wall -Wextra -O3 # Use this to compile normally with full compiller optimizations
HEADERS=HuffmanHeader.h
OBJECT=HuffmanEncoderV2.o

all: HuffmanEncoderV2

HuffmanEncoderV2: $(OBJECT)
	$(CC) -o HuffmanEncoderV2 $(OBJECT) $(CFLAGS)

HuffmanEncoderV2.o: HuffmanEncoderV2.c
	$(CC) $(CFLAGS) -c HuffmanEncoderV2.c

clean:
	-rm -f $(OBJECT) HuffmanEncoderV2