########################################################################
# COSC1076 - Advanced Programming Techniques
# Semester 2 2014 Assignment #2
# Full Name        : Alexander Chi-Cheng Kinross-Smith
# Student Number   : s3603437
# Course Code      : COSC1076
# Program Code     : BH094
# Start up code provided by Paul Miller
########################################################################

SOURCES=ppd_main.c ppd_menu.c ppd_options.c ppd_utility.c ppd_stock.c \
ppd_coin.c
HEADERS=ppd_main.h ppd_menu.h ppd_options.h ppd_utility.h ppd_stock.h \
ppd_coin.h
README=ppd_readme
MAKEFILE=Makefile

COMPILER = gcc

OBJ=ppd_main.o ppd_menu.o ppd_options.o ppd_utility.o ppd_stock.o ppd_coin.o
PROG = ppd
DEBUG = -g
CFLAGS = -Wall -ansi $(DEBUG)

all: $(OBJ)
$(CC) $(CFLAGS) -o $(PROG) $(OBJ)

%.o: $(SOURCES) $(HEADERS)
$(CC) $(CFLAGS) -c -o $@ $^

clean:
@echo "Cleaning output"
rm -f *.o $(PROG)



########################################################################
# Move this target to the end of the Makefile to zip up your code 
# when submitting. Do not submit your .dat files, or directories. 
# We only want the files that are part of your implementation.
########################################################################
archive:
	zip $(USER)-a2 $(SOURCES) $(HEADERS) $(README) $(MAKEFILE)
