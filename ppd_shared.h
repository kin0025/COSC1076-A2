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
#ifndef PPD_SHARED
#define PPD_SHARED
/**
 * datatype to represent a boolean value within the system
 **/
typedef enum truefalse {
    /**
     * the constant for false
     **/
            FALSE,
    /**
     * the constant for true
     **/
            TRUE
} BOOLEAN;

#define MAX_INT_LEN 5
#define EXTRACHARS 2
#define NULL_TERMINATOR '\0'

#ifdef _unix_

#define ENTER_COLOUR "\e[93menter\e[0m"
#define F_LIGHT_YELLOW "\e[93m"
#define F_GREEN "\e[32m"
#define F_LIGHT_GREEN "\e[92m"
#define F_CYAN "\e[97m"
#define F_BLACK "\e[30m"

#define B_GREEN "\e[42m"
#define B_BLUE "\e[44m"
#define B_YELLOW "\e[45m"

#define COLOUR_RESET "\e[0m"


#else


#define ENTER_COLOUR "enter"
#define F_LIGHT_YELLOW ""
#define F_GREEN ""
#define F_LIGHT_GREEN ""
#define F_CYAN ""
#define F_BLACK ""

#define B_GREEN ""
#define B_BLUE ""
#define B_YELLOW ""

#define COLOUR_RESET ""


#endif

#endif
