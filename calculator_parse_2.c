#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "calculator_parse.h"

/**
 * Change from Calculator parse 1 is that we generalize the load function such 
 * that it searches for the highest grammar structure then operates it. This 
 * function will be called load then each of the functions will be called 
 * operate. Using Left Recursion. 
 * 
 * Parser plan is to have operations that call each other recursively 
 */

#define MAX_LOOP 100

double load(char* text, int len);

double op_division(char* text_1, int len_1, char* text_2, int len_2);
double op_multiplication(char* text_1, int len_1, char* text_2, int len_2);
double op_addition(char* text_1, int len_1, char* text_2, int len_2);
double op_subtraction(char* text_1, int len_1, char* text_2, int len_2);
double op_brackets(char* text, int len);
double op_number(char* text, int len);

static int load_level = 0;

/**
 * Calculate is following bidmas but it needs and is folliowing parsing via 
 * recursion. 
 */
double calculate(char* text) {
    printf("Begin Calculate in Calculator parse 2\n");

    int i = 0;
    int len = strlen(text);
    
    // parse
    return load(text, len);
}

/**
 * Searches for the highest order thing then 
 */
double load(char* text, int len) {
    int i = 0;
    load_level++;
    
    // Operation Max
    int op = -1;
    int op_i = 0;
    int op_level = INT_MAX;
    int level = 0;

    
    // loop 
    while (text[i] != '\0' && i < len) {
        // Level management
        if (text[i] == '(') {
            level++;
        } else if (text[i] == ')') {
            level--;
        }


        // Addition 
        if (
            text[i] == '+' &&
            level < op_level &&
            (op == -1 || op == '/' || op == '*' || op == '(')
        ) {
            op = '+';
            op_i = i;
            op_level = level;
        } 
        
        // Subtraction
        if (
            text[i] == '-' &&
            level < op_level &&
            (op == -1 || op == '/' || op == '*' || op == '(')
        ) {
            op = '-';
            op_i = i;
            op_level = level;
        }
        
        // Multiplication
        if (
            text[i] == '*' &&
            level < op_level &&
            (op == -1 || op == '(')
        ) {
            op = '*';
            op_i = i;
            op_level = level;
        } 
        
        
        // Division
        if (
            text[i] == '/' &&
            level < op_level &&
            (op == -1 || op == '(')
        ) {
            op = '/';
            op_i = i;
            op_level = level;
        } 
        
        // Brackets
        if (
            text[i] == '(' &&
            level < op_level &&
            op == -1
        ) {
            op = '(';
            op_i = i;
            op_level = level;
        }
        
        // Iterate 
        i++;
    }
    printf("Calculate: \"%.*s\" with op %c\n", len, text, op != -1 ? op : 'N');
    
    // Addition
    if (op == '+') {
        return op_addition(text, op_i, text + op_i + 1, len - (op_i + 1));
    }
    
    // Subtraction
    if (op == '-') {
        return op_subtraction(text, op_i, text + op_i + 1, len - (op_i + 1));
    }
    
    // Multiplication
    if (op == '*') {
        return op_multiplication(text, op_i, text + op_i + 1, len - (op_i + 1));
    }
    
    // Division
    if (op == '/') {
        return op_division(text, op_i, text + op_i + 1, len - (op_i + 1));
    }
    
    // Brackets
    if (op == '(') {
        return op_brackets(text + op_i, len - op_i);
    }

    // Number
    return op_number(text, len);

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

double op_division(char* text_1, int len_1, char* text_2, int len_2) {
    return load(text_1, len_1) / load(text_2, len_2); 
}

double op_multiplication(char* text_1, int len_1, char* text_2, int len_2) {
    return load(text_1, len_1) * load(text_2, len_2); 
}

double op_addition(char* text_1, int len_1, char* text_2, int len_2) {
    return load(text_1, len_1) + load(text_2, len_2); 
}

double op_subtraction(char* text_1, int len_1, char* text_2, int len_2) {
    return load(text_1, len_1) - load(text_2, len_2); 
}

double op_brackets(char* text, int len) {
    int start_i = 0;
    int end_i = len;
    int level = 0;
    
    for (int i = 0; i < len; i++) {
        if (text[i] == '(') {
            if (level == 0) {
                start_i = i;
            }
            level++;
        } else if (text[i] == ')') {
            if (level == 1) {
                end_i = i;
            }
            level--;
        }
    }
    
    return load(text + start_i + 1, end_i - start_i - 1);
}

double op_number(char* text, int len) {
    int num = 0;

    for (int i = 0; i < len; i++) {
        if ('0' <= text[i] || text[i] <= '9') {
            num = num * 10 + text[i] - '0';
        }
    }
    
    return num;
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