#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <iostream>

#include "functions.h"

int main(int argc, char **argv)
{
    // char expression[MAX] = "";
    // scanf("%s", expression);

    struct Sum global_sum;
    Token tokens[MAX_TOKENS] = {};
    calculate(argv[1], &global_sum, tokens);

    // calculate("[[2, 2, 3], [4, 5, 6], [7, 8, 9]]*[[3, 2, 3], [4, 5, 6], [7, 8, 9]]*[[3, 2, 3], [4, 5, 6], [7, 8, 9]] + [[4, 2, 3], [4, 5, 6], [7, 8, 9]] * [[5, 2, 3], [4, 5, 6], [7, 8, 9]] + [[6, 2, 3], [4, 5, 6], [7, 8, 9]]", &global_sum, tokens);
    // calculate("[[1.21, 1, 1], [1, 1, 1], [1, 1, 1]]+[[1, 1, 1], [1, 1, 1], [1, 1, 1]]", &global_sum, tokens);
    // calculate("[[22, 2, 3], [4, 5, 64], [7, 8, 9]]*[[3, 2, 3], [4, 5, 6], [7, 8, 9]]", &global_sum);
    // calculate("[[22, 2, 3], [4, 5, 64], [7, 8, 9]]+[[3, 2, 3], [4, 5, 6], [7, 8, 9]]", &global_sum);
    return 0;
}