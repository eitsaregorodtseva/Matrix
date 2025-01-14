#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <iostream>

static const int MAX_COLS = 3;
static const int MAX_ROWS = 3;
static const int MAX = 100;
static const int MAX_TOKENS = 2000;
static const int DOT_CODE = 46;

enum class E_TOKEN
{
    COMMA,
    SUM = '+',
    SUB,
    MUL,
    EXP,
    TRANSP,
    OPEN_BRACKET,
    CLOSE_BRACKET,
    NUMBER,
};

struct Token
{
    E_TOKEN type;
    float value;
};

struct Matrix
{
    float data[MAX_ROWS][MAX_COLS];
};

class Matrix1
{
public:
    Matrix1();
    ~Matrix1();
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

class Sum1
{
public:
    Sum1();
    ~Sum1();

    

private:
    struct Mul *muls[MAX] = {};
    int cnt = 0;
};

void calculate(char expression[], struct Sum *global_sum, Token tokens[MAX_TOKENS]);