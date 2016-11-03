#ifndef INSTR_H
#define INSTR_H

#include "error.h"
#include "ial.h"
#include <stdlib.h>
#include <stdio.h>

enum instruction_set {
    I_NOP,
    I_STOP,
    I_IF,
    I_PARAM_ADD,
    I_CALL,
    I_RET,
    I_ELSE_BEGIN,
    I_ELSE_END,
    I_CONDITION_LOOP,
    I_END_LOOP,
    I_ASSIGN,

    I_READLN, // read()
    I_WRITE, // write()

    I_PLUS,
    I_MINUS,
    I_MULTIPLY,
    I_DIVIDE,
    I_LT,
    I_GT,
    I_LTE,
    I_GTE,
    I_EQ,
    I_FUN_START,
    I_NEQ,

    //////////////////////////////////// STRING ////////////////////////////////////

    I_ASSIGN_STRING,

    I_CONCATENATE,
    I_LENGTH,
    I_COPY_STRING,
    I_SORT_STRING,
    I_FIND_STRING,
};

typedef struct {
    enum instruction_set inst_type;
    void *address1; // adresa 1 - typicky operand
    void *address2; // adresa 2 - typicky operand
    void *address3; // adresa 3 - typicky vysledok alebo cielova adresa
} inst_t; // instruction

// Linked list DS for instructions
typedef struct item_inst {
    inst_t inst;
    struct item_inst *next_inst; // next instruction
} item_inst_t;

typedef struct {
    item_inst_t *first; // first instruction
    item_inst_t *last; // last instruction
    item_inst_t *active; // active
} list_inst_t;

extern list_inst_t instruction_list;

void init_list();
void free_list();

item_inst_t *list_append(inst_t instruction);
void list_first();
void list_next();
item_inst_t *add_instruction(enum instruction_set i, void *op1, void *op2, void *op);
inst_t *list_get_inst();

//void list_Goto(void *go_to_Instr);
//void *listGetPointerLast();

inst_t tmp_inst;

#endif