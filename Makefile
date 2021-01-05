all: gmiview

CFLAGS = -Wall

gmiview: gmiview.o
	cc gmiview.o -o gmiview

clean:
	rm -f gmiview *.o
