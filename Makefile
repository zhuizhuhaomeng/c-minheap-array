CFLAGS = -Wall -std=c99 -O2
DEPS=main.o heap.o
DEPS2=main2.o heap.o
PRGM=test
PRGM2=test2

all: test test2
test: $(DEPS)
	$(CC) -g $(DEPS) -o $(PRGM)

test2: $(DEPS2)
	$(CC) -g $(DEPS2) -o $(PRGM2)

clean:
	rm *.o $(PRGM)

