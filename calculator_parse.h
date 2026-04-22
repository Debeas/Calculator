#ifndef CALCULATOR_PARSE_H
#define CALCULATOR_PARSE_H

typedef struct calculator_state {
    char* str;
    int i;
    int len;
} calculator_state_t;

double calculate(char* text);
double load_number(char* text, int len);
double load_dm(char* text, int len);
double load_as(char* text, int len);

calculator_state_t* calculator_state_create();
void calculator_state_free(calculator_state_t* cs);
void calculator_button_character(calculator_state_t* cs, char c);

#endif