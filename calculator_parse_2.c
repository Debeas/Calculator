#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "calculator_parse.h"

/**
 * Change from Calculator parse 1 is that we generalize the load function such 
 * that it searches for the highest grammar structure then operates it. This 
 * function will be called load then each of the functions will be called 
 * operate. Using Left Recursion. 
 */

#define MAX_LOOP 100

/**
 * Calculate is following bidmas but it needs and is folliowing parsing via 
 * recursion. 
 */
double calculate(char* text) {
    printf("Begin Calculate\n");

    int i = 0;
    int len = strlen(text);
    
    // parse
    return load_as(text, len);
}

/**
 * Searches for the highest order thing then 
 */
double load(char* text, int len) {

}

double load_number(char* text, int len) {

    // variables
    int i = 0;
    int num = 0;
    while (text[i] != '\0' && i < len && !('0' <= text[i] && text[i] <= '9')) i++;
    while (text[i] != '\0' && i < len && ('0' <= text[i] && text[i] <= '9')) {
        num = num * 10 + text[i] - '0';
        i++;
    }
    return num;
}

/**
 * Sweep for the division multiplication
 */
double load_dm(char* text, int len) {
    // index
    int i = 0;
    int start_i = 0;
    int end_i = len;
    
    // num
    double hold_num = 0;
    double out_num = 1;
    
    // operation
    int last_op = 0;
    int no_op = 0;
    
    // loop
    while (text[i] != '\0' && i < len) {
        if (text[i] == '*' || text[i] == '/') {
            // Recurse
            end_i = i;
            hold_num = load_number(text + start_i, end_i - start_i);
            
            // Operation
            if (no_op != 0) {
                if (last_op == '*') {
                    out_num *= hold_num;
                } else if (last_op == '/') {
                    out_num /= hold_num;
                }
            } else {
                out_num = hold_num;
            }
            
            // Iterate
            last_op = text[i];
            no_op++;
            start_i = i + 1;
            end_i = len;
        } 
        i++;
    }
    
    // Final Load
    hold_num = load_number(text + start_i, end_i - start_i);
    
    
    // Operation
    if (no_op != 0) {
        if (last_op == '*') {
            out_num *= hold_num;
        } else if (last_op == '/') {
            out_num /= hold_num;
        }
    } else {
        out_num = hold_num;
    }
    
    return out_num;
}

double load_as(char* text, int len) {
    // index
    int i = 0;
    int start_i = 0;
    int end_i = len;
    
    // num
    double hold_num = 0;
    double out_num = 1;
    
    // operation
    int last_op = 0;
    int no_op = 0;
    int last_char_num = 0==1;
    
    // loop
    while (text[i] != '\0' && i < len) {
        if (text[i] == '+' || text[i] == '-') {
            // Recurse
            end_i = i;
            hold_num = load_dm(text + start_i, end_i - start_i);
            
            // Operation
            if (no_op != 0) {
                if (last_op == '+') {
                    out_num += hold_num;
                } else if (last_op == '-') {
                    out_num -= hold_num;
                }
            } else {
                out_num = hold_num;
            }
            
            // Iterate
            last_op = text[i];
            no_op++;
            start_i = i + 1;
            end_i = len;
        } 
        if ('0' <= text[i] && text[i] <= '9') {
            last_char_num = (1==1);
        } else {
            last_char_num = (0==1);
        }
        i++;
    }
    
    // Final Load
    hold_num = load_dm(text + start_i, end_i - start_i);
    
    // Operation
    if (no_op != 0) {
        if (last_op == '+') {
            out_num += hold_num;
        } else if (last_op == '-') {
            out_num -= hold_num;
        }
    } else {
        out_num = hold_num;
    }
    
    return out_num;
}

/**
 * Does the bracket of bidmas, needs to check for different levels of brackets
 * unsolved problem. 
 */
double load_b(char* text, int len) {
    // index
    int i = 0;
    int start_i = 0;
    int end_i = len;
    
    // num
    double hold_num = 0;
    double out_num = 1;
    
    // operation
    int last_op = 0;
    int no_op = 0;
    int last_char_num = 0==1;
    int level = 0;

    // loop
    while (text[i] != '\0' && i < len) {
        if (text[i] == '(' || text[i] == ')') {
            
            // Highest level only
            if (text[i] == '(') {
                level++;
            } else if (text[i] == ')') {
                level--;
            }
            
            // Recurse
            end_i = i;
            hold_num = load_dm(text + start_i, end_i - start_i);
            
            // Operation
            if (no_op != 0) {
                if (last_op == '+') {
                    out_num += hold_num;
                } else if (last_op == '-') {
                    out_num -= hold_num;
                }
            } else {
                out_num = hold_num;
            }
            
            // Iterate
            last_op = text[i];
            no_op++;
            start_i = i + 1;
            end_i = len;
        } 
        if ('0' <= text[i] && text[i] <= '9') {
            last_char_num = (1==1);
        } else {
            last_char_num = (0==1);
        }
        i++;
    }
    
    // Final Load
    hold_num = load_dm(text + start_i, end_i - start_i);
    
    // Operation
    if (no_op != 0) {
        if (last_op == '+') {
            out_num += hold_num;
        } else if (last_op == '-') {
            out_num -= hold_num;
        }
    } else {
        out_num = hold_num;
    }
    
    return out_num;
}

#pragma region Calculator State 


calculator_state_t* calculator_state_create() {
    calculator_state_t* cs  = malloc(sizeof(calculator_state_t));
    if (cs == NULL) {
        printf("Failure");
        exit(1);
    }
    cs->i = 0;
    cs->str = malloc(101);
    cs->len = 101;
    cs->str[0] = '\0';
    return cs;
}
void calculator_state_free(calculator_state_t* cs) {
    free(cs->str);
    free(cs);
}
void calculator_button_character(calculator_state_t* cs, char c) {
    if (cs->i < cs->len) {
        cs->str[cs->i++] = c;
        cs->str[cs->i] = '\0';
    }
}


#pragma endregion Calculator State 