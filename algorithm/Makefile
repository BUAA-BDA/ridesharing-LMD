##
#	Makefile for the manuscript submitted to VLDB'20,
#	Author: Trasier
##
CC = gcc
CXX = g++
LIBS = 
CFLAGS = --std=c++11 -W -Wall -Wno-sign-compare -O3 -s -pipe -mmmx -msse -msse2 -msse3 # -mcmodel=medium
MEM = -D WATCH_MEM

all: fesif chst
global.o: global.h global.cpp
	$(CXX) $(CFLAGS) -c global.cpp $(LIBS)
	
HST.o: HST.h HST.cpp global.h global.cpp global.o
	$(CXX) $(CFLAGS) -c global.o HST.cpp $(LIBS)
	
utils.o: utils.h utils.cpp HST.h HST.cpp HST.o global.h global.cpp global.o
	$(CXX) $(CFLAGS) -c global.o HST.o utils.cpp $(LIBS)
	
chst: constructHSTs.cpp global.h global.cpp global.o HST.h HST.cpp HST.o
	$(CXX) $(CFLAGS) -o chst global.o HST.o constructHSTs.cpp $(LIBS) $(MEM)

fesif: fesif.cpp utils.h utils.cpp utils.o HST.h HST.cpp HST.o global.h global.cpp global.o
	$(CXX) $(CFLAGS) -o fesif utils.o global.o HST.o fesif.cpp $(LIBS) $(MEM)
	
.PHONY: clean
clean:
		-@rm *.o *.gcno *~ 2> /dev/null || true
		-@rm fesif chst 2> /dev/null || true
