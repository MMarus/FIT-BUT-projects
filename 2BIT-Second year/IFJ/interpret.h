#ifndef INTERPRET_H
#define INTERPRET_H

#include <stdlib.h>
#include <stdbool.h>
#include "instr.h"
#include "ial.h"
#include "i_string.h"
#include "parser.h"

typedef struct
{
    int cond;
} condition;

struct if_stack
{
    int top;
    int size;
    condition **cond;
};

int interpret();

extern list_inst_t instruction_list;

#endif