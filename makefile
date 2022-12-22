# Makefile for VWAP TrexQuant Assignment
 
# *****************************************************
# Variables
 
CC = clang++
CFLAGS =-g -std=c++11 -Wall -pedantic
 
# ****************************************************
# Targets 

main: main.o Utils.o
	$(CC) $(CFLAGS) -o main main.o Utils.o
 
# The main.o target can be written more simply
main.o: main.cpp Header.h
	$(CC) $(CFLAGS) -c main.cpp

Utils.o: Utils.cpp Header.h
	$(CC) $(CFLAGS) -c Utils.cpp
	
# clean:
# 	rm -rf Utils.o
# 	rm -rf main.o