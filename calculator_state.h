#ifndef CALCULATOR_STATE_H
#define CALCULATOR_STATE_H

typedef struct calculator_state {
    char* text;
    int text_size;
    int text_i;

    struct calculator_function* head;
} calculator_state_t;

typedef struct parser_state {
    struct parser_node** node_arr;
    int node_arr_size;
    int node_arr_i;
} parser_state_t;

typedef struct parser_node {
    char c;
    enum parser_node_type type;
} parser_node_t;

// typedef struct parser_end {

// } parser_end_t;

typedef struct calculator_function {
    int num_i;
    int num1;
    int num2;
    struct calculator_function* root1;
    struct calculator_function* root2;
    enum calculator_function_type type;
} calculator_function_t;

typedef enum calculator_function_type {
    UNKNOWN,
    ADDITION,
    SUBTRACT,
    MULTIPLICATION,
    DIVISION
} calculator_function_type_t;

typedef enum parser_node_type {
    OPERATOR,
    DIGIT,
    BRACKET
} parser_node_type_t;

void calculator_state_free(calculator_state_t* cs);
calculator_state_t* calculator_state_create();

void calculator_button_character(calculator_state_t* cs, char b);

calculator_function_t* calculator_function_create(calculator_function_type_t type);
void calculator_function_free(calculator_function_t* cf);
double calculator_function_execute(calculator_function_t* cf);
void calculator_function_write_num(calculator_function_t* cf, char c);

void parser_node_append(parser_state_t* ps, char c, parser_node_type_t pnt);
void parser_node_action(calculator_state_t* cs, parser_state_t* ps, parser_node_t* pn);

double parser(calculator_state_t* cs, parser_state_t* ps);

parser_state_t* parser_state_create();
parser_node_t* parser_check(parser_state_t* ps, char c);
#endif