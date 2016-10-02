/***********************************************************************
 * COSC1076 - Advanced Programming Techniques
 * Semester 2 2016 Assignment #2
 * Full Name        : Alexander Chi-Cheng Kinross-Smith
 * Student Number   : s3603437
 * Course Code      : COSC1076
 * Program Code     : BH094
 * Start up code provided by Paul Miller
 * Some codes are adopted here with permission by an anonymous author
 ***********************************************************************/

#include <stdio.h>
#include "ppd_main.h"

#ifndef PPD_UTILITY
#define PPD_UTILITY
#define DATA_DELIMITER "|"
/**
 * @file ppd_utility.h defines some helper functions for the rest of your
 * program. In particular, it declares three of the most important 
 * functions for your application: @ref load_data which loads the data
 * in from files, @ref system_init which initialises the system to a 
 * known safe state and @ref system_free which frees all memory when you
 * are going to quit the application.
 **/

/**
 * the function to call for buffer clearing. This was discussed extensively
 * for assignment 1
 **/
void read_rest_of_line(void);

/**
 * Initialise the system to a known safe state. Before you post on the
 * discussion board about this, have a look at the structures as defined
 * in ppd_stock.h, ppd_coin.h and ppd_main.h. Given that nothing is 
 * initialized by the system if it
 * is a local variable, what would be good starting values for each of 
 * these. You already have some experience with this from assignment 
 * 1 to help you.
 **/
BOOLEAN system_init(struct ppd_system *);

/**
 * loads the stock file's data into the system. This needs to be 
 * implemented as part of requirement 2 of the assignment specification.
 **/
BOOLEAN load_stock(struct ppd_system *, const char *);

/**
 * loads the contents of the coins file into the system. This needs to
 * be implemented as part 1 of requirement 18.
 **/
BOOLEAN load_coins(struct ppd_system *, const char *);


#define BASE 10

#define NUMBER_STOCK_DELIMS 4

#define NUMBER_COIN_DELIMS 1

#define MAX_DOLLARS_PRICE 100

/**
 * free all memory that has been allocated. If you are struggling to
 * find all your memory leaks, compile your program with the -g flag
 * and run it through valgrind. An important thing to think about here:
 * as malloc() returns a memory address to the first byte allocated, you
 * must pass each of these memory addresses to free, and no other 
 * memory addresses.
 **/
void system_free(struct ppd_system *);

BOOLEAN read_int(int *output);

BOOLEAN to_int(char *input, int *output);

BOOLEAN read_user_input(char *buffer, int length);

BOOLEAN read_file_input(char *buffer, int length, FILE *file);

BOOLEAN string_to_price(struct price *price_amount, char *price_input);

BOOLEAN price_to_int(struct price *price, int *cents);

BOOLEAN rename_file(const char *name, BOOLEAN reverse);

int count_delims(char *delim, char *string);

BOOLEAN print_error_message(char *field_type, int line_number, const char
*file_name);
#endif
