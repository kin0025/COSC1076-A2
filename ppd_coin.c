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
#include "ppd_coin.h"

 /**
  * @file ppd_coin.c implement functions here for managing coins in the
  * "cash register" contained in the @ref ppd_system struct.
  **/
BOOLEAN void_balances(struct coin cash_register[NUM_DENOMS]) {
    int i;
    for (i = 0; i < NUM_DENOMS; i++) {
       cash_register[i]->denom = i;
       cash_register[i]->count = 0;
    }
    return TRUE;
 }
 BOOLEAN is_valid_denom(int denom){
    int valid_denoms[NUM_DENOMS] = VALID_DENOMS,i;
    BOOLEAN is_valid = FALSE;
    for(i = 0;i<valid_denoms;i++){
       if(denom[i] == denom){
          is_valid = TRUE;
       }
    }
    return is_valid;
 }
struct price coins_to_price(int cents){
   struct price return_val;
   return_val.dollars -= (int)cents/CENTSINDOLLAR;
   return_val.cents -= (int)cents - (return_val.dollars*CENTSINDOLLAR);
   return return_val;
}