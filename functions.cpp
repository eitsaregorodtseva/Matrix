#include "functions.h"

int tokenize_string(char expression[], Token *tokens)
{

    int size = 0;
    int i = 0;
    char tmp_str[MAX] = "";

    while (i < strlen(expression))
    {
        if ((isdigit(expression[i])) || (expression[i] == DOT_CODE))
        {
            tmp_str[strlen(tmp_str)] = expression[i];
        }
        else
        {
            if (strlen(tmp_str) > 0)
            {
                tokens[size] = {
                    .type = E_TOKEN::NUMBER,
                    .value = (float)atof(tmp_str)};
                size++;
                // i++;
                memset(tmp_str, '\0', sizeof(tmp_str));
            }
        }

        switch (expression[i])
        {
        case '[':
            tokens[size] = {
                .type = E_TOKEN::OPEN_BRACKET,
            };
            size++;
            break;

        case ']':
            tokens[size] = {
                .type = E_TOKEN::CLOSE_BRACKET,
            };
            size++;
            break;

        case '+':
            tokens[size] = {
                .type = E_TOKEN::SUM,
            };
            size++;
            break;

        case '-':
            tokens[size] = {
                .type = E_TOKEN::SUB,
            };
            size++;
            break;

        case '*':
            tokens[size] = {
                .type = E_TOKEN::MUL,
            };
            size++;
            break;

        case ',':
            tokens[size] = {
                .type = E_TOKEN::COMMA,
            };
            size++;
            break;
        }

        i++;
    }

    return size;
}
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

void parse_matrix(Token tokens[MAX_TOKENS], struct Matrix *matrix, int start_ind, int end_ind)
{
    int col = 0;
    int row = 0;

    for (int ind = start_ind; ind < end_ind; ind++)
    {
        if (tokens[ind].type == E_TOKEN::NUMBER)
        {
            matrix->data[row][col] = tokens[ind].value;
            col++;
            if (col == MAX_COLS)
            {
                col = 0;
                row++;
            }
        }
    }
    // print_result_matrix(matrix->data);
}

void parse_mul(Token tokens[MAX_TOKENS], struct Mul *mul, int start, int end)
{
    mul->cnt = 0;
    int start_ind = start;

    for (int ind = start; ind < end; ind++)
    {
        if (tokens[ind].type == E_TOKEN::MUL)
        {
            mul->matrices[mul->cnt] = new Matrix;
            parse_matrix(tokens, mul->matrices[mul->cnt], start_ind, ind - 1);
            start_ind = ++ind;
            mul->cnt++;
        }
    }

    if ((tokens[start_ind - 1].type == E_TOKEN::MUL) | (start_ind == start))
    {
        mul->matrices[mul->cnt] = new Matrix;
        parse_matrix(tokens, mul->matrices[mul->cnt], start_ind, end);
        mul->cnt++;
    }
}

void parse_sum(Token tokens[MAX_TOKENS], int size, struct Sum *global_sum)
{
    global_sum->cnt = 0;
    int start_ind = 0;
    int curr_ind = 0;

    for (int i = 0; i < size - 1; i++)
    {
        if (tokens[i].type == E_TOKEN::SUM)
        {
            global_sum->muls[global_sum->cnt] = new Mul;
            parse_mul(tokens, global_sum->muls[global_sum->cnt], start_ind, curr_ind - 1);
            start_ind = ++curr_ind;
            global_sum->cnt++;
        }
        curr_ind++;
    }

    if ((tokens[start_ind - 1].type == E_TOKEN::SUM) | (start_ind == 0))
    {
        global_sum->muls[global_sum->cnt] = new Mul;
        parse_mul(tokens, global_sum->muls[global_sum->cnt], start_ind, curr_ind);
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

void calculate(char expression[], struct Sum *global_sum, Token tokens[MAX_TOKENS])
{
    int size = tokenize_string(expression, tokens);
    parse_sum(tokens, size, global_sum);
    calculate_muls(global_sum);
    sum_matrices(global_sum);
}