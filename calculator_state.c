/**
 * Calculator Screen 50 characters
 * 
 * Recursive descent parsing or Shunting Yard
 * 
 * 
 * 
 */
#include <stdio.h>
#include <stdlib.h>
#include "calculator_state.h"

#define TEST_CALCULATOR_STATE 
#define ALLOC_CHECK(thing) if (thing == NULL) { printf("Failed Malloc on line %d\n", __LINE__); exit(0); }
#define FREE_CHECK(thing) if (thing != NULL) {free(thing); thing = NULL;}


#pragma region Calculator State

calculator_state_t* calculator_state_create() {
    calculator_state_t* cs = malloc(sizeof(calculator_state_t));
    ALLOC_CHECK(cs);

    // Text
    cs->text_size = 50;
    cs->text_i = 0;
    cs->text = malloc(cs->text_size * sizeof(char));
    ALLOC_CHECK(cs->text);

    // head
    cs->head = NULL;
    
    return cs;
}

void calculator_button_character(calculator_state_t* cs, char b) {
    if (cs->text_i < cs->text_size) {
        cs->text[cs->text_i++] = b;
        cs->text[cs->text_i] = '\0';
        printf("cs->text: %s\n", cs->text);
    }
}

void calculator_state_free(calculator_state_t* cs) {
    FREE_CHECK(cs->text);
    FREE_CHECK(cs);
}


#pragma endregion Calculator State


#pragma region Parser
/**
 * Following BIDMAS
 *  - Brackets
 *  - Indices
 *  - Division/Multiplicagtion 
 *  - Addition/Subtraction
 *  - NUMBERS
 */
double parser(calculator_state_t* cs, parser_state_t* ps) {
    for (int i = 0; i < cs->text_i; i++) {
        parser_node_t* pn = parser_check(ps, cs->text[i]);

        parser_node_action(cs, ps, pn);
    }    

    double value = calculator_function_execute(cs->head);
    calculator_function_free(cs->head);
    return value;
}

parser_state_t* parser_state_create() {
    parser_state_t* ps = malloc(sizeof(parser_state_t));
    ALLOC_CHECK(ps);
    ps->node_arr_i = 0;
    ps->node_arr_size = 2;
    ps->node_arr = malloc(sizeof(parser_node_t*) * ps->node_arr_size);

    // Parser Node
    parser_node_append(ps, '0', DIGIT);
    parser_node_append(ps, '1', DIGIT);
    parser_node_append(ps, '2', DIGIT);
    parser_node_append(ps, '3', DIGIT);
    parser_node_append(ps, '4', DIGIT);
    parser_node_append(ps, '5', DIGIT);
    parser_node_append(ps, '6', DIGIT);
    parser_node_append(ps, '7', DIGIT);
    parser_node_append(ps, '8', DIGIT);
    parser_node_append(ps, '9', DIGIT);
    parser_node_append(ps, '+', OPERATOR);
    parser_node_append(ps, '-', OPERATOR);
    parser_node_append(ps, '*', OPERATOR);
    parser_node_append(ps, '/', OPERATOR);
    parser_node_append(ps, '(', BRACKET);
    parser_node_append(ps, ')', BRACKET);

    // 

    return ps;
}


void parser_state_free(parser_state_t* ps) {
    FREE_CHECK(ps);
}

void parser_node_append(parser_state_t* ps, char c, parser_node_type_t pnt) {
    
    // Increase Array
    while (ps->node_arr_i >= ps->node_arr_size) {
        ps->node_arr_size = ps->node_arr_size == 0 ? 1 : 2 * ps->node_arr_size;
        parser_node_t** pn_arr_new = malloc(sizeof(parser_node_t*) * ps->node_arr_size);
        ALLOC_CHECK(pn_arr_new)
        for (int i = 0; i < ps->node_arr_size && i < ps->node_arr_i; i++) {
            pn_arr_new[i] = ps->node_arr[i];
        }
        FREE_CHECK(ps->node_arr)
        ps->node_arr = pn_arr_new;
    }

    
    
}

parser_node_t* parser_node_create(char c, parser_node_type_t type) {
    parser_node_t* pn = malloc(sizeof(parser_node_t));
    ALLOC_CHECK(pn);

    pn->c = c;
    pn->type = type;

    return pn;
}

void parser_node_free(parser_node_t* pn) {
    FREE_CHECK(pn);

}

/**
 * BIDMAS is important here
 * DM above AS such that when it occurs do not know how to handle
 */
void parser_node_action(calculator_state_t* cs, parser_state_t* ps, parser_node_t* pn) {
    if (cs->head == NULL) {
        if (pn->type == DIGIT) {
            cs->head = calculator_function_create(UNKNOWN);
            cs->head->num1 = pn->c - '0';
        } else if (pn->type == OPERATOR) {
            if (pn->c == '-') {
                cs->head = calculator_function_create(SUBTRACT);
            } else if (pn->c == '+') {
                cs->head = calculator_function_create(ADDITION);
            } else {
                printf("Error Operator before thing\n");
                exit(0);
            }
        } else {
            printf("Error Operator before thing\n");
            exit(0);
        }
    }

    if (pn->type == DIGIT) {
        if (cs->head->type == ADDITION || cs->head->type == SUBTRACT) {
            calculator_function_write_num(cs->head, pn->c);
        }
    } else if (pn->type == OPERATOR) {
        if (pn->c == '-' || pn->c == '+') {
            if (cs->head->type == SUBTRACT || cs->head->type == ADDITION) {
                // No change in operation order
                calculator_function_t* new_head = calculator_function_create(pn->c == '-' ? SUBTRACT : ADDITION);
                new_head->root1 = cs->head;
                cs->head = new_head;
            } else if (cs->head->type == MULTIPLICATION || cs->head->type == DIVISION) {
                // No change in operation order
                calculator_function_t* new_head = calculator_function_create(pn->c == '-' ? SUBTRACT : ADDITION);
                new_head->root1 = cs->head;
                cs->head = new_head;
            }
        } else if (pn->c == '*' || pn->c == '/') {
            if (cs->head->type == MULTIPLICATION || cs->head->type == DIVISION) {
                // No change in operation order
                calculator_function_t* new_head = calculator_function_create(pn->c == '*' ? MULTIPLICATION : DIVISION);
                new_head->root1 = cs->head;
                cs->head = new_head;
            } else if (cs->head->type == SUBTRACT || cs->head->type == ADDITION) {
                // No change in operation order
                calculator_function_t* new_head = calculator_function_create(pn->c == '-' ? SUBTRACT : ADDITION);
                new_head->root1 = cs->head;
                cs->head = new_head;
            }
        } 
    }   

}


parser_node_t* parser_check(parser_state_t* ps, char c) {
    for (int i = 0; i < ps->node_arr_i; i++) {
        if (ps->node_arr[i]->c == c) return ps->node_arr[i];
    }
    return NULL;
}

#pragma endregion Parser

#pragma region Calculator Function

calculator_function_t* calculator_function_create(calculator_function_type_t type) {
    calculator_function_t* cf = malloc(sizeof(calculator_function_t));
    ALLOC_CHECK(cf);
    cf->type = type;
    cf->num1 = 0;
    cf->num2 = 0;
    cf->root1 = NULL;
    cf->root2 = NULL;
    
    return cf;
}

void calculator_function_free(calculator_function_t* cf) {
    if (cf->root1 != NULL) calculator_function_free(cf);
    if (cf->root2 != NULL) calculator_function_free(cf);
    FREE_CHECK(cf);
}

void calculator_function_write_num(calculator_function_t* cf, char c) {
    if (cf->root2 == NULL) {
        if (cf->num_i == 1) {
            cf->num1 = cf->num1 * 10 + c - '0';
        }
    } else calculator_function_write_num(cf->root2, c);
}

double calculator_function_execute(calculator_function_t* cf) {
    double value = 0;
    if (cf->type == ADDITION) {
        if (cf->root1 == NULL) {
            value +=cf->num1;
        } else {
            value += calculator_function_execute(cf->root1);
        }
        if (cf->root2 == NULL) {
            value +=cf->num2;
        } else {
            value += calculator_function_execute(cf->root2);
        }
    } else if (cf->type == SUBTRACT) {
        if (cf->root1 == NULL) {
            value +=cf->num1;
        } else {
            value += calculator_function_execute(cf->root1);
        }
        if (cf->root2 == NULL) {
            value -=cf->num2;
        } else {
            value -= calculator_function_execute(cf->root2);
        }
    } else if (cf->type == MULTIPLICATION) {
        if (cf->root1 == NULL) {
            value +=cf->num1;
        } else {
            value += calculator_function_execute(cf->root1);
        }
        if (cf->root2 == NULL) {
            value *=cf->num2;
        } else {
            value *= calculator_function_execute(cf->root2);
        }
    } else if (cf->type == DIVISION) {
        if (cf->root1 == NULL) {
            value +=cf->num1;
        } else {
            value += calculator_function_execute(cf->root1);
        }
        if (cf->root2 == NULL) {
            value /=cf->num2;
        } else {
            value /= calculator_function_execute(cf->root2);
        }
    }
    return value;
}

#pragma endregion Calculator Function