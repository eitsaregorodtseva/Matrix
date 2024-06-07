#include <stdio.h>
#include <string.h>
#include <stdlib.h>

static const int MAX_COLS = 3;
static const int MAX_ROWS = 3;
static const int MAX = 100;
static const int DOT_CODE = 46;

struct Matrix
{
    float data[MAX_ROWS][MAX_COLS];
};

struct Mul
{
    struct Matrix *matrices[MAX];
    int cnt;
};

struct Sum
{
    struct Mul *muls[MAX];
    int cnt;
};

void print_result_matrix(float result[][MAX_COLS])
{
    printf("[");
    for (int i = 0; i < MAX_ROWS; i++)
    {
        printf("[");
        for (int j = 0; j < MAX_COLS; j++)
        {
            if (j == MAX_COLS - 1)
            {
                printf("%.2f", result[i][j]);
            }
            else
            {
                printf("%.2f, ", result[i][j]);
            }
        }
        if (i == MAX_ROWS - 1)
        {
            printf("]");
        }
        else
        {
            printf("], ");
        }
    }
    printf("]");
    printf("\n");
}

void parse_matrix(char expression[], struct Matrix *matrix, char *start_ptr, char *end_ptr)
{
    char tmp_str[MAX] = "";
    int col = 0;
    int row = 0;
    
    for (char *ptr = start_ptr; ptr < end_ptr; ptr++)
    {
        if ((atoi(ptr)) | (*ptr == DOT_CODE))
        {
            tmp_str[strlen(tmp_str)] = *ptr;
        }
        else
        {
            if (strlen(tmp_str) > 0)
            {
                matrix->data[row][col] = (float)atof(tmp_str);
                memset(tmp_str, '\0', sizeof(tmp_str));

                col++;
                if (col == MAX_COLS)
                {
                    col = 0;
                    row++;
                }
            }
        }
    }
    // print_result_matrix(matrix->data);
}

void parse_mul(char expression[], struct Mul *mul, char *start, char *end)
{
    mul->cnt = 0;
    char *start_ptr = start;

    for (char *ptr = start; ptr < end; ptr++)
    {
        if (*ptr == '*')
        {
            mul->matrices[mul->cnt] = (Matrix *)malloc(sizeof(struct Matrix));
            parse_matrix(expression, mul->matrices[mul->cnt], start_ptr, ptr - 1);
            start_ptr = ++ptr;
            mul->cnt++;
        }
    }
    
    if ((*(start_ptr - 1) == '*') | (start == start_ptr))
    {
        mul->matrices[mul->cnt] = (Matrix *)malloc(sizeof(struct Matrix));
        parse_matrix(expression, mul->matrices[mul->cnt], start_ptr, end);
        mul->cnt++;
    }
}

void parse_sum(char expression[], struct Sum *global_sum)
{
    global_sum->cnt = 0;
    char *start_ptr = expression;
    char *curr_ptr = expression;

    for (int i = 0; i < strlen(expression); i++)
    {
        if (*curr_ptr == '+')
        {
            global_sum->muls[global_sum->cnt] = (Mul *)malloc(sizeof(struct Mul));
            parse_mul(expression, global_sum->muls[global_sum->cnt], start_ptr, curr_ptr - 1);
            start_ptr = ++curr_ptr;
            global_sum->cnt++;
        }
        curr_ptr++;
    }

    if ((*(start_ptr - 1) == '+') | (start_ptr == expression))
    {
        global_sum->muls[global_sum->cnt] = (Mul *)malloc(sizeof(struct Mul));
        parse_mul(expression, global_sum->muls[global_sum->cnt], start_ptr, curr_ptr);
        global_sum->cnt++;
    }
}

void multiply_matrices(struct Matrix *first_matrix, struct Matrix *second_matrix, float result[][MAX_COLS])
{
    for (int i = 0; i < MAX_ROWS; i++)
    {
        for (int j = 0; j < MAX_COLS; j++)
        {
            result[i][j] = 0;
            for (int k = 0; k < MAX_ROWS; k++)
            {
                result[i][j] += first_matrix->data[i][k] * second_matrix->data[k][j];
            }
        }
    }
}

void sum_matrices(struct Sum *global_sum)
{
    float result[MAX_ROWS][MAX_COLS] = {0};

    for (int i = 0; i < global_sum->cnt; i++)
    {
        for (int j = 0; j < MAX_ROWS; j++)
        {
            for (int k = 0; k < MAX_COLS; k++)
            {
                result[j][k] += global_sum->muls[i]->matrices[0]->data[j][k];
            }
        }
    }

    print_result_matrix(result);
}

void calculate_muls(struct Sum *global_sum)
{
    for (int i = 0; i < global_sum->cnt; i++)
    {
        if (global_sum->muls[i]->cnt > 1)
        {
            float result[MAX_ROWS][MAX_COLS];
            for (int j = 0; j < global_sum->muls[i]->cnt; j++)
            {
                if (j == 0)
                {
                    multiply_matrices(global_sum->muls[i]->matrices[j], global_sum->muls[i]->matrices[j + 1], result);
                }
                else
                {
                    multiply_matrices(global_sum->muls[i]->matrices[0], global_sum->muls[i]->matrices[j + 1], result);
                }
                memcpy(global_sum->muls[i]->matrices[0]->data, result, sizeof(struct Matrix));
                global_sum->muls[i]->cnt--;
            }
        }
    }
}

void calculate(char expression[], struct Sum *global_sum)
{
    parse_sum(expression, global_sum);
    calculate_muls(global_sum);
    sum_matrices(global_sum);
}

int main(int argc, char **argv)
{
    // char expression[MAX] = "";
    // scanf("%s", expression);

    struct Sum global_sum;
    calculate(argv[1], &global_sum);
    
    // calculate("[[1.21, 1, 1], [1, 1, 1], [1, 1, 1]]+[[1, 1, 1], [1, 1, 1], [1, 1, 1]]", &global_sum);
    // calculate("[[22, 2, 3], [4, 5, 64], [7, 8, 9]]*[[3, 2, 3], [4, 5, 6], [7, 8, 9]]", &global_sum);
    // calculate("[[22, 2, 3], [4, 5, 64], [7, 8, 9]]+[[3, 2, 3], [4, 5, 6], [7, 8, 9]]", &global_sum);
    // calculate("[[2, 2, 3], [4, 5, 6], [7, 8, 9]]*[[3, 2, 3], [4, 5, 6], [7, 8, 9]]*[[3, 2, 3], [4, 5, 6], [7, 8, 9]] + [[4, 2, 3], [4, 5, 6], [7, 8, 9]] * [[5, 2, 3], [4, 5, 6], [7, 8, 9]] + [[6, 2, 3], [4, 5, 6], [7, 8, 9]]", &global_sum);
    return 0;
}