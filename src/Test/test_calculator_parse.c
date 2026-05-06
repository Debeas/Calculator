#include <stdio.h>
#include <stdlib.h>

#include "..\calculator_parse.h"

typedef struct test {
    char* text;
    double score;
} test_t;

static test_t text_tests[] = {
  { .text = "1 + 1", .score = 2},
  { .text = "2 * 8", .score = 16},
  { .text = "6 - 0", .score = 6},
  { .text = "4 / 2", .score = 2},
  { .text = "5 + 7 * 2 - 4", .score = 15},
  { .text = "15 - 10 * -2", .score = 35},
  { .text = "1050 + 7 - 56 * 2", .score = 945},
  { .text = "50 / 2 / 5 * 3 - -10", .score = 25},
  { .text = "0 + 1 - -2 * -3", .score = -5},
  { .text = "14 / 2 + 15 / 3", .score = 12},
  { .text = "5 / 2 + 6 / 2", .score = 5.5},
};

int main() {
    printf("Begin Test\n");
    int len = sizeof(text_tests) / sizeof(test_t);
    double score = 0.;
    int correct = 0;
    for (int i = 0; i < len; i++) {
        score = calculate(text_tests[i].text);
        printf("Testing %d as %.2lf == %.2lf %s\n", i, score, text_tests[i].score, score == text_tests[i].score ? "PASS" : "FAIL");
        printf("\t%s\n", text_tests[i].text);
        if (score == text_tests[i].score) correct ++;
    }   
    printf("Tests out of %d/%d\n", correct, len);
}
