#MAKEFILE for Nonequilbirium Propogator routine
#
#Steven Large
#JApril 5th 2018

PRODUCT = Diffusion.exe

MAINFILE = DiffusionCoefficient.cpp

CFLAGS = -I.

CC = g++ -std=c++11 -O3

all: $(PRODUCT)

$(PRODUCT): $(MAINFILE)
	$(CC) $(MAINFILE) -o $(PRODUCT) $(CFLAGS)

clean:
	rm -f *.o

clean_ex:
	rm -f *.exe

