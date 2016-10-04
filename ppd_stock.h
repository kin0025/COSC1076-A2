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
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <ctype.h>
#include "ppd_coin.h"
#include "ppd_shared.h"

#ifndef PPD_STOCK
#define PPD_STOCK

/**
 * @file ppd_stock.h this file defines the data structures required to 
 * manage the stock list. You should add here the function prototypes for
 * managing the list here and implement them in ppd_stock.c
 **/

/**
 * The length of the id string not counting the nul terminator
 **/
#define IDLEN 5

/**
 * The maximum length of a product name not counting the nul terminator
 **/
#define NAMELEN 40

/**
 * The maximum length of a product description not counting the nul
 * terminator.
 **/
#define DESCLEN 255

/**
 * The maximum string length of a cost inclusing delimiter
 */
#define COSTLEN 6

/**
 * The number of delims in a single saved stock entry
 */
#define NUMBERDELIMS 5

/**
 * The maximum string length of on hand
 */
#define ONHANDLEN 3

/**
 * The maximum length of a line in the save file
 */
#define FILE_LINE_LEN (IDLEN+NAMELEN+DESCLEN+COSTLEN+ONHANDLEN+NUMBERDELIMS)

/**
 * The default coin level to reset the coins to on request
 **/
#define DEFAULT_COIN_COUNT 20

/**
 * The default stock level that all new stock should start at and that 
 * we should reset to on restock
 **/
#define DEFAULT_STOCK_LEVEL 20


/**
 * a structure to represent a price. One of the problems with the floating
 * point formats in C like float and double is that they have minor issues
 * of inaccuracy due to rounding. In the case of currency this really is
 * not acceptable so we introduce our own type to keep track of currency.
 **/
struct price {
    /**
     * the dollar value for some price
     **/
    unsigned dollars,
    /**
     * the cents value for some price
     **/
            cents;
};

/**
 * data structure to represent a stock item within the system
 **/
struct ppd_stock {
    /**
     * the unique id for this item
     **/
    char id[IDLEN + 1];
    /**
     * the name of this item
     **/
    char name[NAMELEN + 1];
    /**
     * the description of this item
     **/
    char desc[DESCLEN + 1];
    /**
     * the price of this item
     **/
    struct price price;
    /**
     * how many of this item do we have on hand? 
     **/
    unsigned on_hand;
    /**
     * a pointer to the next node in the list
     **/
};

/**
 * the node that holds the data about an item stored in memory
 **/
struct ppd_node {
    /* pointer to the data held for the node */
    struct ppd_stock *data;
    /* pointer to the next node in the list */
    struct ppd_node *next;
};

/**
 * the list of products - each link is the list is a @ref ppd_node
 **/
struct ppd_list {
    /**
     * the beginning of the list
     **/
    struct ppd_node *head;
    /**
     * how many nodes are there in the list?
     **/
    unsigned count;
};

/**
 * this is the header structure for all the datatypes that is 
 * passed around and manipulated
 **/
struct ppd_system {
    /**
     * the container for all the money manipulated by the system
     **/
    struct coin cash_register[NUM_DENOMS];

    /**
     * the linked list - note that this is a pointer - how does that
     * change what we need to do for initialization of the list?
     **/
    struct ppd_list *item_list;

    /**
     * the name of the coin file - we need this for saving as all menu
     * items only take the one parameter of the ppd_system
     **/
    const char *coin_file_name;
    /**
     * the name of the stock file
     **/
    const char *stock_file_name;

    /* are the coins loaded in from a file ? */
    BOOLEAN coin_from_file;
};

/**
 * Initialises the list to empty values
 * @param system The system that contains the list to be initialised
 * @return whether the list was properly initialised
 */
BOOLEAN init_list(struct ppd_system *system);

/**
 * Adding item stock to the item_list enclosed in system in alphavetical order.
 * @param stock A populated stock item to be added to the list
 * @param system The system to add the item to
 * @return true if the item was added, false if any error was encountered
 * whilst adding.
 */
BOOLEAN add_stock(struct ppd_stock stock, struct ppd_system *system);

/**
 * Removes stock from based on an ID given
 * @param system The system that contains the item_list that we are searching in
 * @param id The id that we are searching for
 * @return Whether the item was found and deleted
 */
BOOLEAN remove_stock(struct ppd_system *system, char id[IDLEN + 1]);

/**
 * Returns the length of the largest description in the system's list
 * @param system The system that contains the list
 * @return the length of the larges object
 */
int get_largest_description(struct ppd_system *system);

/**
 * Returns the length of the largest name in the system's list
 * @param system The system that contains the list
 * @return the length of the largest object
 */
int get_largest_name(struct ppd_system *system);

/**
 * Returns the number of the next ID that hasn't been used
 * @param system The system to search through
 * @return the next unused/never used(assuming the largest ID node never gets
 * deleted, as we have no system to store these) id number
 */
int get_next_id(struct ppd_system *system);

/**
 * Finds the stock that owns the ID and returns it
 * @param system The system item that contains the item list to search through
 * @param id The id to search for
 * @return NULL if nothing found. The found stock item
 */
struct ppd_stock *find_id(struct ppd_system *system, char *id);

/**
 * Mallocs a node and returns if it worked
 * @return The memory pointer that was malloced
 */
struct ppd_node *create_node(void);

/**
 * Frees the memory of a node
 * @param node The node that needs to have its memory freed. Should have been
 * malloced prior
 */
void del_node(struct ppd_node *node);

/**
 * Converts a stock item into a delimited, formatted string
 * @param string The string to be output into
 * @param node The node to make the string out of
 * @return Whether a string was made
 */
BOOLEAN stock_to_string(char string[FILE_LINE_LEN], struct ppd_node *node);

/**
 * Save all the items to the file located in the system for data
 * @param system Contains the file name to save to and the list to be saved
 * @return Whether the save completed
 */
BOOLEAN save_stock(struct ppd_system *system);

/** Prints a single stock item. No validation needed or occurs*/
void print_stock(struct ppd_stock stock_item);

/**
 * Checks for a next node and increments the current node in the lsit if
 * possible.
 * @param node The node to be incremented
 * @return
 */
BOOLEAN next_node(struct ppd_node **node);

/**
 * Initialises a node to the head position of the item_list
 * @param node The node to be initialised to a position
 * @param system The system that contains the list
 * @return Whether the initialisation completed
 */
BOOLEAN init_node(struct ppd_node **node, struct ppd_system *system);

/**
 * Checks if an ID is syntaxically valid
 * @param id The id to check
 * @param system the system to check for duplicates in
 * @return Whether the ID is valid or not
 */
BOOLEAN is_valid_id(char *id, struct ppd_system *system);

#endif


