#include "instr.h"

inline item_inst_t *add_instruction(enum instruction_set i, void *op1, void *op2, void *op)
{
#ifdef DEBUG_I
    printf("add_instruction(), inst_type: %d, %p, %p, %p\n", i, op1, op2, op);
    fflush(stream);
#endif

    tmp_inst.inst_type = i;
    tmp_inst.address1 = op1;
    tmp_inst.address2 = op2;
    tmp_inst.address3 = op;
    return list_append(tmp_inst);
}

inline void init_list()
{
    instruction_list.first = NULL;
    instruction_list.last = NULL;
    instruction_list.active = NULL;
}

void free_list()
{
    item_inst_t *iter_tmp = instruction_list.first;
    item_inst_t *to_free;

    while (iter_tmp) {
        to_free = iter_tmp;
        iter_tmp = iter_tmp->next_inst;

        free(to_free);
    }

    init_list(); // resets
}

item_inst_t *list_append(inst_t instruction)
{
    item_inst_t *new_inst = malloc(sizeof(item_inst_t));
    new_inst->inst = instruction;
    new_inst->next_inst = NULL;

    if (instruction_list.first != NULL)
        instruction_list.last->next_inst = new_inst;
    else
        instruction_list.first = new_inst;

    instruction_list.last = new_inst;
#ifdef DEBUG
    printf("list_append() | inst_type: %d\n", new_inst->inst.inst_type);
#endif
    return new_inst;
}

inline void list_first()
{
    instruction_list.active = instruction_list.first;
}

inline void list_next()
{
    if (instruction_list.active != NULL)
        instruction_list.active = instruction_list.active->next_inst;
}

inline inst_t *list_get_inst()
{
    if (instruction_list.active != NULL)
        return &(instruction_list.active->inst);
    else
        err(INTERNAL_ERR, "list_get_inst() | There is no active instruction");

    return NULL;
}