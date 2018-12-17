CC=g++
CFLAGS=-I$(IDIR) -std=c++11 -g
OPENCV=`pkg-config --cflags opencv` `pkg-config --libs opencv`


ODIR=../src
IDIR =../include

_OBJ = main.o HandGesture.o MyBGSubtractorColor.o
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))

all: main

$(ODIR)/HandGesture.o: $(ODIR)/HandGesture.cpp
	$(CC) -c -o $@ $< $(CFLAGS) $(OPENCV)

$(ODIR)/MyBGSubtractorColor.o: $(ODIR)/MyBGSubtractorColor.cpp
	$(CC) -c -o $@ $< $(CFLAGS) $(OPENCV)

$(ODIR)/main.o: $(ODIR)/main.cpp
	$(CC) -c -o $@ $< $(CFLAGS) $(OPENCV)

main: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS) $(OPENCV)

.PHONY: clean

clean:
	rm -f $(ODIR)/*.o
