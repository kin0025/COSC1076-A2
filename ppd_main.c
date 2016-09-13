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

#include "ppd_main.h"
#include "ppd_menu.h"
#include "ppd_options.h"
#include "ppd_utility.h"

/**
 * @file ppd_main.c contains the main function implementation and any 
 * helper functions for main such as a display_usage() function.
 **/

/**
 * manages the running of the program, initialises data structures, loads
 * data and handles the processing of options. The bulk of this function
 * should simply be calling other functions to get the job done.
 **/
int main(int argc, char **argv)
{
    /* uncomment this menu structure when you are ready to work on the 
     * menu system with function pointers
    struct menu_item menu[NUM_MENU_ITEMS];
    */
    /* validate command line arguments */

    /* represents the data structures to manage the system */
    struct ppd_system system;
    (void)system;
    /* init the system */

    /* load data */

    /* test if everything has been initialised correctly */

    /* initialise the menu system */

    /* loop, asking for options from the menu */

    /* run each option selected */

    /* until the user quits */

    /* make sure you always free all memory and close all files 
     * before you exit the program
     */

    return EXIT_SUCCESS;
}
