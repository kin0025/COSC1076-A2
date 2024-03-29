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

#include "ppd_main.h"
/**
 * @file ppd_menu.h defines the @ref menu_item type and the init_menu 
 * function which you need to call to initialise it
 **/
#ifndef MENU_H
#define MENU_H
struct ppd_system;


/**
 * The maximum length of a menu item's text
 **/
#define MENU_NAME_LEN 50

/**
 * The number of items in the menu
*/
#define NUM_MENU_ITEMS 9
/**
 * The names of each menu item
 */
#define MENU_NAMES {"Display Items","Purchase Items","Save and Exit","Add "\
"Item","Remove Item","Display Coins","Reset Stock","Reset Coins","Abort "\
"Program"}

/**
 * The functions called when each menu item is selected.
 */
#define MENU_FUNCTIONS {&display_items,&purchase_item,&save_system,&add_item,\
&remove_item,&display_coins,&reset_stock,&reset_coins,NULL}

/* represents a function that can be selected from the list of
 * menu_functions - creates a new type called a menu_function
 */
typedef BOOLEAN (*menu_function)(struct ppd_system *);

/**
 * represents a menu item to be displayed and executed in the program
 **/
struct menu_item {
    /**
     * the text to be displayed in the menu
     **/
    char name[MENU_NAME_LEN + 1];
    /**
     * pointer to the function to be called when this item is selected
     **/
    menu_function function;
};

/**
 * In this function you need to initialise the array of menu items 
 * according to the text to be displayed for the menu. This array is 
 * an array of @ref menu_item with a name and a pointer to the function
 * that will be called for that function. 
 *
 * Please note that you are expected to initialise the code in such a way
 * that the code will be easy to maintain. Do not hard code values, and 
 * do not use magic numbers for array indexes etc. You are expected to use
 * good coding practices at all times. Do not hardcode array indexes - 
 * you are expected to write code that is maintainable which in this 
 * case means initialization in a loop.
 **/
void init_menu(struct menu_item *);

/**
 * Prints all the options, returns a value and requests input.
 * @return the menu function to run.
 */
menu_function get_menu_choice(struct menu_item *);

#endif
