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

/**
 * @file ppd_coin.h defines the coin structure for managing currency in the
 * system. You should declare function prototypes for managing coins here
 * and implement them in ppd_coin.c
 **/
#include "ppd_shared.h"

#ifndef PPD_COIN
#define PPD_COIN
struct ppd_system;
struct price;

/** The delminiter in files for coin value and coin count **/
#define COIN_DELIM ","
/** The number of cents in one dollar **/
#define CENTS_IN_DOLLAR 100

/** Array of values of denoms in order of denom enum **/
#define VALID_DENOMS {5,10,20,50,100,200,500,1000}

/** The minimum cents value of a denom **/
#define MINUMUM_DENOM 5

/**
 * The number of denominations of currency available in the system
 **/
#define NUM_DENOMS 8

/**
 * The maximum padding of the left side of the coin display
 */
#define DISPLAY_LEFT_PADDING 11
/**
 * The maximum length of one line in a coin file
 */
#define COIN_LINE_LEN 8
/**
 * The delimiter between dollars and cents in string form prices
 */
#define PRICEDELIM "."
/**
 * The maximum count of coins the inventory can hold
 */
#define MAX_COIN_COUNT 99
/**
 * enumeration representing the various types of currency available in
 * the system. 
 **/
enum denomination {
    FIVE_CENTS, TEN_CENTS, TWENTY_CENTS, FIFTY_CENTS, ONE_DOLLAR,
    TWO_DOLLARS, FIVE_DOLLARS, TEN_DOLLARS
};

/**
 * enumeration representing the typed of denominations
 */
enum denom_type {
    DOLLARS, CENTS
};

/**
 * represents a coin type stored in the cash register. Each demonination
 * will have exactly one of these in the cash register.
 **/
struct coin {
    /**
     * the denomination type
     **/
    enum denomination denom;
    /**
     * the count of how many of these are in the cash register
     **/
    unsigned count;
};

/** Sets balances in cash_register to zero and sets the denoms to descending
 * values. */
BOOLEAN void_balances(struct coin cash_register[NUM_DENOMS]);

/** Resets all coins to the DEFAULT coin count.*/
void reset_coins_imp(struct ppd_system *system);

/**
 * Add amount of coins from the cash register passed in of the type
 * denom.
 */
BOOLEAN add_coin_denom(struct coin cash_register[NUM_DENOMS], enum
        denomination denom, int amount);

/**
 * Removes amount of coins from the cash register passed in of the type
 * denom. */
BOOLEAN remove_coin_denom(struct coin cash_register[NUM_DENOMS], enum
        denomination denom, int amount);

/** A wrapper function for add_coin_denom() that takes values instead **/
BOOLEAN add_coin_val(struct coin cash_register[NUM_DENOMS], int value, int
amount);

/** A wrapper function for remove_coin_denom() that takes values instead **/
BOOLEAN remove_coin_val(struct coin cash_register[NUM_DENOMS], int value, int
amount);

/** Checks if the value passed in is a valid denomination*/
BOOLEAN is_valid_value(int value);

/** Checks if the denomination passed in is a valid one that we have a value
 * for.*/
BOOLEAN is_valid_denom(enum denomination denom);

/* Returns the number of coins of type denom in the cash register */
int count_coins(struct coin cash_register[NUM_DENOMS], enum denomination denom);

/** Converts a cents value of coins into a price value with seperate cents
 * dollar amounts, and returns it. */
struct price coins_to_price(int cents);

/** Displays the coins in the order they are in the cash register */
void display_coins_imp(struct ppd_system *system);

/** Save the coins to a file and return whether it succeed */
BOOLEAN save_coins(struct ppd_system *system);

/** Calculates the change required for the coin amount (change_amount),
 * using the change values in the ppd_system. The values are returned in the
 * coin_change array. */
BOOLEAN calculate_change(struct coin change[NUM_DENOMS], int change_amount,
                         struct ppd_system *system);

/** Counts the number of decimal places the int has */
int num_places(int n);

/** Get the type of denom from the unit value*/
enum denom_type type_of_denom(unsigned int *unit_value);

/** Converts a value in cents to the appropriate denomination */
enum denomination value_to_denom(int value);

/** Returns the cents value of the denom inputted*/
int denom_valuer(enum denomination denom);

#endif
