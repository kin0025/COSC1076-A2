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
#define COIN_DELIM ","
struct ppd_system;
struct price;

#define CENTS_IN_DOLLAR 100

#define VALID_DENOMS {5,10,20,50,100,200,500,1000}

/**
 * The number of denominations of currency available in the system
 **/
#define NUM_DENOMS 8

/**
 * enumeration representing the various types of currency available in
 * the system. 
 **/
enum denomination {
    FIVE_CENTS, TEN_CENTS, TWENTY_CENTS, FIFTY_CENTS, ONE_DOLLAR,
    TWO_DOLLARS, FIVE_DOLLARS, TEN_DOLLARS
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



BOOLEAN void_balances(struct coin cash_register[NUM_DENOMS]);

struct price coins_to_price(int cents);

BOOLEAN is_valid_denom(int denom);

#endif
