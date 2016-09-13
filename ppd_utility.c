/***********************************************************************
 * COSC1076 - Advanced Programming Techniques
 * Semester 2 2016 Assignment #2
 * Full Name        : EDIT HERE
 * Student Number   : EDIT HERE
 * Course Code      : EDIT HERE
 * Program Code     : EDIT HERE
 * Start up code provided by Paul Miller
 * Some codes are adopted here with permission by an anonymous author
 ***********************************************************************/

#include "ppd_utility.h"
/**
 * @file ppd_utility.c contains implementations of important functions for
 * the system. If you are not sure where to implement a new function, 
 * here is probably a good spot.
 **/

void read_rest_of_line(void)
{
    int ch;
    /* keep retrieving characters from stdin until we
     * are at the end of the buffer
     */
    while(ch = getc(stdin), ch!='\n' && ch != EOF)
        ;
    /* reset error flags on stdin */
    clearerr(stdin);
}

/**
 * @param system a pointer to a @ref ppd_system struct that holds all 
 * the data for the system we are creating
 **/
BOOLEAN system_init(struct ppd_system * system)
{
    /*
     * Please delete this default return value once this function has 
     * been implemented. Please note that it is convention that until
     * a function has been implemented it should return FALSE
     */
    return FALSE;
}

/**
 * loads the stock file's data into the system. This needs to be 
 * implemented as part of requirement 2 of the assignment specification.
 **/
BOOLEAN load_stock(struct ppd_system * system, const char * filename)
{
    /*
     * Please delete this default return value once this function has 
     * been implemented. Please note that it is convention that until
     * a function has been implemented it should return FALSE
     */
    return FALSE;
}

/**
 * loads the contents of the coins file into the system. This needs to
 * be implemented as part 1 of requirement 18.
 **/
BOOLEAN load_coins(struct ppd_system *, const char *)
{
    /*
     * Please delete this default return value once this function has 
     * been implemented. Please note that it is convention that until
     * a function has been implemented it should return FALSE
     */
    return FALSE;

}

/**
 * @param system a pointer to a @ref ppd_system struct that holds all 
 * the data for the system we are creating
 **/
void system_free(struct ppd_system * system)
{

}
