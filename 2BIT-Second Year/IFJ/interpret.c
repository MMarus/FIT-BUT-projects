#include "interpret.h"

void p_check_type(pair_t *p, int type1, int type2)
{
    if (p->token_type != type1 && p->token_type != type2)
    {
        err(SEMANTIC_TYPE, "wrong type");
    }
}

void s_check_type(Symbol_t s, int type)
{
    if (!s->defined)
    {
        err(RUNTIME_UNITIALIZED_ERR, "unitialized variable");
    }

    if (s->type != type)
    {
        err(SEMANTIC_TYPE, "wrong type");
    }
}

void init_if_stack(struct if_stack *if_stack1)
{
    if_stack1->top = 0;
    if_stack1->size = 8;
    if_stack1->cond = malloc(if_stack1->size * sizeof(condition*));
    if (if_stack1->cond == NULL)
    {
        err(INTERNAL_ERR, "Couldnt get memory");
    }
    if_stack1->cond[if_stack1->top] = malloc(sizeof(condition));
    if (if_stack1->cond[if_stack1->top] == NULL)
    {
        err(INTERNAL_ERR, "Couldnt get memory");
    }
    if_stack1->cond[if_stack1->top]->cond = 2;

    //printf("PUSH\t%d == %d\n", if_stack1->top, if_stack1->cond[if_stack1->top]->cond);
}

void if_stack_push(struct if_stack *if_stack1, int if_condition)
{
    if_stack1->top += 1;
    if_stack1->cond[if_stack1->top] = malloc(sizeof(condition));
    if (if_stack1->cond[if_stack1->top] == NULL)
    {
        err(INTERNAL_ERR, "Couldnt get memory");
    }
    if_stack1->cond[if_stack1->top]->cond = if_condition;

    //printf("PUSH\t%d == %d\n", if_stack1->top, if_stack1->cond[if_stack1->top]->cond);
}

void if_stack_pop(struct if_stack *if_stack1, int *if_condition)
{
    *if_condition = if_stack1->cond[if_stack1->top]->cond;
    if (if_stack1->top > 0)
    {
        if_stack1->top -= 1;
    }

    //printf("POP \t%d == %d\n", if_stack1->top, if_stack1->cond[if_stack1->top]->cond);
}

int if_stack_top(struct if_stack *if_stack1)
{
    return if_stack1->cond[if_stack1->top]->cond;
}

int interpret()
{
    int if_condition = 2; // 0 - true, 1 - false, 2 - none
    int if_condition_state = 2; // 0 - if, 1 - else, 2 - none
    int loop_condition = 2; // 0 - true, 1 - false, 2 - none

    item_inst_t *loop_inst = NULL;
    token_t *t;
    token_t *_op1, *_op2;
    value_u *u;
    string_t none;
    Symbol_t s, op, in, out;
    int type;
    double op1, op2;
    char *tmp, *tmp1, *tmp2, *end;
    function_env_t *f_tmp, *f_t;
    pair_t *_in;

    none.name = "none";
    none.length = 0;

    struct if_stack if_stack1;
    init_if_stack(&if_stack1);

    while (true)
    {
        int instruction = instruction_list.active->inst.inst_type;

        if (if_condition != if_condition_state && instruction != I_ELSE_BEGIN && instruction != I_ELSE_END && instruction != I_IF)
        {
            instruction = I_NOP;
        }

        if (loop_condition == 1 && instruction != I_END_LOOP)
        {
            instruction = I_NOP;
        }

        switch (instruction)
        {
            case I_NOP:
                break;
            case I_WRITE:
                u = (value_u *)instruction_list.active->inst.address3;
                type = (int)instruction_list.active->inst.address2;
                s = (Symbol_t)instruction_list.active->inst.address1;

                if (s != NULL && !s->defined)
                {
                    err(RUNTIME_UNITIALIZED_ERR, "Unitialized variable");
                }

                if (type == K_STRING)
                {
                    printf("%s", u->string->name);
                }
                else if (type == K_INTEGER)
                {
                    printf("%d", u->i);
                }
                else if (type == K_REAL)
                {
                    printf("%g", u->d);
                }
                else if (type == K_BOOLEAN)
                {
                    if (u->b)
                    {
                        printf("TRUE");
                    }
                    else
                    {
                        printf("FALSE");
                    }
                }
                else
                {
                    err(RUNTIME_OTHER_ERR, "Unknown type");
                }

                break;
            case I_READLN:
                s = (Symbol_t) instruction_list.active->inst.address3;
                s->data->string = malloc(sizeof(string_t));
                s->data->string->name = load_string(stdin);
                s->defined = true;

                if (s->type == K_INTEGER)
                {
                    int base = s->data->string->name[0];
                    if (base == '%')
                    {
                        s->data->i = (int)strtol(&s->data->string->name[1], &tmp, 2);
                        if (*tmp != '\0')
                        {
                            err(RUNTIME_LOAD_ERR, "Expected number");
                        }
                    }
                    else if (base == '&')
                    {
                        s->data->i = (int)strtol(&s->data->string->name[1], &tmp, 8);
                        if (*tmp != '\0')
                        {
                            err(RUNTIME_LOAD_ERR, "Expected number");
                        }
                    }
                    else if (base == '$')
                    {
                        s->data->i = strtol(&s->data->string->name[1], &tmp, 16);
                        if (*tmp != '\0')
                        {
                            err(RUNTIME_LOAD_ERR, "Expected number");
                        }
                    }
                    else
                    {
                        s->data->i = strtol(s->data->string->name, &tmp, 10);
                        if (*tmp != '\0')
                        {
                            err(RUNTIME_LOAD_ERR, "Expected number");
                        }
                    }
                }
                else if (s->type == K_REAL)
                {
                    s->data->d = strtod(s->data->string->name, NULL);
                    free(s->data->string->name);
                    free(s->data->string);
                }

                break;
            case I_ASSIGN:
                s = (Symbol_t) instruction_list.active->inst.address1;
                _op2 = (token_t*)instruction_list.active->inst.address3;

                if (_op2->type == ID)
                {
                    op = find_symbol(_op2->str);
                    _op2->type = op->type;
                    _op2->tdata = *op->data;
                }

                if (convert_to_k_type(s->type) != convert_to_k_type(_op2->type))
                {
                    err(SEMANTIC_TYPE, "Incompatible types");
                }

                if (s->type == K_INTEGER)
                {
                    s->data->i = _op2->tdata.i;
                }
                else if(s->type == K_REAL)
                {
                    s->data->d = _op2->tdata.d;
                }
                else if(s->type == K_BOOLEAN)
                {
                    s->data->b = _op2->tdata.b;
                }
                else if(s->type == convert_to_k_type(K_STRING))
                {
                    s->data->string = _op2->tdata.string;
                }

                s->defined = true;

                break;
            case I_PLUS:
                _op1 = (token_t*)instruction_list.active->inst.address1;
                _op2 = (token_t*)instruction_list.active->inst.address2;
                t = (token_t*)instruction_list.active->inst.address3;

                op = find_symbol(_op1->str);
                if (op != NULL)
                {
                    if (!op->defined)
                    {
                        err(RUNTIME_UNITIALIZED_ERR, "Unitialized variable");
                    }

                    if (op->type == K_INTEGER)
                    {
                        op1 = op->data->i;
                    }
                    else if (op->type == K_REAL)
                    {
                        op1 = op->data->d;
                    }
                }
                else
                {
                    if (convert_to_k_type(t->type) == K_INTEGER)
                    {
                        op1 = _op1->tdata.i;
                    }
                    else if (convert_to_k_type(t->type) == K_REAL)
                    {
                        op1 = _op1->tdata.d;
                    }
                }

                op = find_symbol(_op2->str);
                if (op != NULL)
                {
                    if (!op->defined)
                    {
                        err(RUNTIME_UNITIALIZED_ERR, "Unitialized variable");
                    }

                    if (op->type == K_INTEGER)
                    {
                        op2 = op->data->i;
                    }
                    else if (op->type == K_REAL)
                    {
                        op2 = op->data->d;
                    }
                }
                else
                {
                    if (convert_to_k_type(t->type) == K_INTEGER)
                    {
                        op2 = _op2->tdata.i;
                    }
                    else if (convert_to_k_type(t->type) == K_REAL)
                    {
                        op2 = _op2->tdata.d;
                    }
                }

                if (convert_to_k_type(t->type) == K_INTEGER)
                {
                    t->tdata.i = op1 + op2;
                }
                else if (convert_to_k_type(t->type) == K_REAL)
                {
                    t->tdata.d = op1 + op2;
                }
                t->str = make_string(&none);

                if (instruction_list.active->next_inst->next_inst->inst.inst_type == I_CONDITION_LOOP)
                {
                    loop_inst = instruction_list.active;
                }

                break;
            case I_MINUS:
                _op1 = (token_t*)instruction_list.active->inst.address1;
                _op2 = (token_t*)instruction_list.active->inst.address2;
                t = (token_t*)instruction_list.active->inst.address3;

                op = find_symbol(_op1->str);
                if (op != NULL)
                {
                    if (!op->defined)
                    {
                        err(RUNTIME_UNITIALIZED_ERR, "Unitialized variable");
                    }

                    if (op->type == K_INTEGER)
                    {
                        op1 = op->data->i;
                    }
                    else if (op->type == K_REAL)
                    {
                        op1 = op->data->d;
                    }
                }
                else
                {
                    if (convert_to_k_type(t->type) == K_INTEGER)
                    {
                        op1 = _op1->tdata.i;
                    }
                    else if (convert_to_k_type(t->type) == K_REAL)
                    {
                        op1 = _op1->tdata.d;
                    }
                }

                op = find_symbol(_op2->str);
                if (op != NULL)
                {
                    if (!op->defined)
                    {
                        err(RUNTIME_UNITIALIZED_ERR, "Unitialized variable");
                    }

                    if (op->type == K_INTEGER)
                    {
                        op2 = op->data->i;
                    }
                    else if (op->type == K_REAL)
                    {
                        op2 = op->data->d;
                    }
                }
                else
                {
                    if (convert_to_k_type(t->type) == K_INTEGER)
                    {
                        op2 = _op2->tdata.i;
                    }
                    else if (convert_to_k_type(t->type) == K_REAL)
                    {
                        op2 = _op2->tdata.d;
                    }
                }

                if (convert_to_k_type(t->type) == K_INTEGER)
                {
                    t->tdata.i = op1 - op2;
                }
                else if (convert_to_k_type(t->type) == K_REAL)
                {
                    t->tdata.d = op1 - op2;
                }
                t->str = make_string(&none);

                if (instruction_list.active->next_inst->next_inst->inst.inst_type == I_CONDITION_LOOP)
                {
                    loop_inst = instruction_list.active;
                }

                break;
            case I_MULTIPLY:
                _op1 = (token_t*)instruction_list.active->inst.address1;
                _op2 = (token_t*)instruction_list.active->inst.address2;
                t = (token_t*)instruction_list.active->inst.address3;

                op = find_symbol(_op1->str);
                if (op != NULL)
                {
                    if (!op->defined)
                    {
                        err(RUNTIME_UNITIALIZED_ERR, "Unitialized variable");
                    }

                    if (op->type == K_INTEGER)
                    {
                        op1 = op->data->i;
                    }
                    else if (op->type == K_REAL)
                    {
                        op1 = op->data->d;
                    }
                }
                else
                {
                    if (convert_to_k_type(t->type) == K_INTEGER)
                    {
                        op1 = _op1->tdata.i;
                    }
                    else if (convert_to_k_type(t->type) == K_REAL)
                    {
                        op1 = _op1->tdata.d;
                    }
                }

                op = find_symbol(_op2->str);
                if (op != NULL)
                {
                    if (!op->defined)
                    {
                        err(RUNTIME_UNITIALIZED_ERR, "Unitialized variable");
                    }

                    if (op->type == K_INTEGER)
                    {
                        op2 = op->data->i;
                    }
                    else if (op->type == K_REAL)
                    {
                        op2 = op->data->d;
                    }
                }
                else
                {
                    if (convert_to_k_type(t->type) == K_INTEGER)
                    {
                        op2 = _op2->tdata.i;
                    }
                    else if (convert_to_k_type(t->type) == K_REAL)
                    {
                        op2 = _op2->tdata.d;
                    }
                }

                if (convert_to_k_type(t->type) == K_INTEGER)
                {
                    t->tdata.i = op1 * op2;
                }
                else if (convert_to_k_type(t->type) == K_REAL)
                {
                    t->tdata.d = op1 * op2;
                }
                t->str = make_string(&none);

                if (instruction_list.active->next_inst->next_inst->inst.inst_type == I_CONDITION_LOOP)
                {
                    loop_inst = instruction_list.active;
                }

                break;
            case I_DIVIDE:
                _op1 = (token_t*)instruction_list.active->inst.address1;
                _op2 = (token_t*)instruction_list.active->inst.address2;
                t = (token_t*)instruction_list.active->inst.address3;

                op = find_symbol(_op1->str);
                if (op != NULL)
                {
                    if (!op->defined)
                    {
                        err(RUNTIME_UNITIALIZED_ERR, "Unitialized variable");
                    }

                    if (op->type == K_INTEGER)
                    {
                        op1 = op->data->i;
                    }
                    else if (op->type == K_REAL)
                    {
                        op1 = op->data->d;
                    }
                }
                else
                {
                    if (convert_to_k_type(t->type) == K_INTEGER)
                    {
                        op1 = _op1->tdata.i;
                    }
                    else if (convert_to_k_type(t->type) == K_REAL)
                    {
                        op1 = _op1->tdata.d;
                    }
                }

                op = find_symbol(_op2->str);
                if (op != NULL)
                {
                    if (!op->defined)
                    {
                        err(RUNTIME_UNITIALIZED_ERR, "Unitialized variable");
                    }

                    if (op->type == K_INTEGER)
                    {
                        op2 = op->data->i;
                    }
                    else if (op->type == K_REAL)
                    {
                        op2 = op->data->d;
                    }
                }
                else
                {
                    if (convert_to_k_type(t->type) == K_INTEGER)
                    {
                        op2 = _op2->tdata.i;
                    }
                    else if (convert_to_k_type(t->type) == K_REAL)
                    {
                        op2 = _op2->tdata.d;
                    }
                }

                if (op2 == 0)
                {
                    err(RUNTIME_DIV_ZERO_ERR, "Divide by zero");
                }

                t->tdata.d = op1 / op2;
                t->str = make_string(&none);

                if (instruction_list.active->next_inst->next_inst->inst.inst_type == I_CONDITION_LOOP)
                {
                    loop_inst = instruction_list.active;
                }

                break;
            case I_LT:
                _op1 = (token_t*)instruction_list.active->inst.address1;
                _op2 = (token_t*)instruction_list.active->inst.address2;
                t = (token_t*)instruction_list.active->inst.address3;

                tmp1 = NULL;
                tmp2 = NULL;

                op = find_symbol(_op1->str);
                if (op != NULL)
                {
                    if (!op->defined)
                    {
                        err(RUNTIME_UNITIALIZED_ERR, "Unitialized variable");
                    }

                    if (op->type == K_INTEGER)
                    {
                        op1 = op->data->i;
                    }
                    else if (op->type == K_REAL)
                    {
                        op1 = op->data->d;
                    }
                    else if (op->type == K_STRING)
                    {
                        tmp1 = op->data->string->name;
                    }
                }
                else
                {
                    if (convert_to_k_type(_op1->type) == K_INTEGER)
                    {
                        op1 = _op1->tdata.i;
                    }
                    else if (convert_to_k_type(_op1->type) == K_REAL)
                    {
                        op1 = _op1->tdata.d;
                    }
                    else if (convert_to_k_type(_op1->type) == K_STRING)
                    {
                        tmp1 = _op1->tdata.string->name;
                    }
                }

                op = find_symbol(_op2->str);
                if (op != NULL)
                {
                    if (!op->defined)
                    {
                        err(RUNTIME_UNITIALIZED_ERR, "Unitialized variable");
                    }

                    if (op->type == K_INTEGER)
                    {
                        op2 = op->data->i;
                    }
                    else if (op->type == K_REAL)
                    {
                        op2 = op->data->d;
                    }
                    else if (op->type == K_STRING)
                    {
                        tmp2 = op->data->string->name;
                    }
                }
                else
                {
                    if (convert_to_k_type(_op2->type) == K_INTEGER)
                    {
                        op2 = _op2->tdata.i;
                    }
                    else if (convert_to_k_type(_op2->type) == K_REAL)
                    {
                        op2 = _op2->tdata.d;
                    }
                    else if (convert_to_k_type(_op1->type) == K_STRING)
                    {
                        tmp2 = _op1->tdata.string->name;
                    }
                }

                if (op1 < op2)
                {
                    t->tdata.b = true;
                }
                else
                {
                    t->tdata.b = false;
                }

                if (tmp1 != NULL && tmp2 != NULL)
                {
                    if (strcmp(tmp1, tmp2) < 0)
                    {
                        t->tdata.b = true;
                    }
                    else
                    {
                        t->tdata.b = false;
                    }
                }

                t->type = K_BOOLEAN;

                loop_inst = instruction_list.active;

                break;
            case I_LTE:
                _op1 = (token_t*)instruction_list.active->inst.address1;
                _op2 = (token_t*)instruction_list.active->inst.address2;
                t = (token_t*)instruction_list.active->inst.address3;

                tmp1 = NULL;
                tmp2 = NULL;

                op = find_symbol(_op1->str);
                if (op != NULL)
                {
                    if (!op->defined)
                    {
                        err(RUNTIME_UNITIALIZED_ERR, "Unitialized variable");
                    }

                    if (op->type == K_INTEGER)
                    {
                        op1 = op->data->i;
                    }
                    else if (op->type == K_REAL)
                    {
                        op1 = op->data->d;
                    }
                    else if (op->type == K_STRING)
                    {
                        tmp1 = op->data->string->name;
                    }
                }
                else
                {
                    if (convert_to_k_type(_op1->type) == K_INTEGER)
                    {
                        op1 = _op1->tdata.i;
                    }
                    else if (convert_to_k_type(_op1->type) == K_REAL)
                    {
                        op1 = _op1->tdata.d;
                    }
                    else if (convert_to_k_type(_op1->type) == K_STRING)
                    {
                        tmp1 = _op1->tdata.string->name;
                    }
                }

                op = find_symbol(_op2->str);
                if (op != NULL)
                {
                    if (!op->defined)
                    {
                        err(RUNTIME_UNITIALIZED_ERR, "Unitialized variable");
                    }

                    if (op->type == K_INTEGER)
                    {
                        op2 = op->data->i;
                    }
                    else if (op->type == K_REAL)
                    {
                        op2 = op->data->d;
                    }
                    else if (op->type == K_STRING)
                    {
                        tmp2 = op->data->string->name;
                    }
                }
                else
                {
                    if (convert_to_k_type(_op2->type) == K_INTEGER)
                    {
                        op2 = _op2->tdata.i;
                    }
                    else if (convert_to_k_type(_op2->type) == K_REAL)
                    {
                        op2 = _op2->tdata.d;
                    }
                    else if (convert_to_k_type(_op1->type) == K_STRING)
                    {
                        tmp2 = _op1->tdata.string->name;
                    }
                }

                if (op1 <= op2)
                {
                    t->tdata.b = true;
                }
                else
                {
                    t->tdata.b = false;
                }

                if (tmp1 != NULL && tmp2 != NULL)
                {
                    if (strcmp(tmp1, tmp2) <= 0)
                    {
                        t->tdata.b = true;
                    }
                    else
                    {
                        t->tdata.b = false;
                    }
                }

                t->type = K_BOOLEAN;

                loop_inst = instruction_list.active;

                break;
            case I_GT:
                _op1 = (token_t*)instruction_list.active->inst.address1;
                _op2 = (token_t*)instruction_list.active->inst.address2;
                t = (token_t*)instruction_list.active->inst.address3;

                tmp1 = NULL;
                tmp2 = NULL;

                op = find_symbol(_op1->str);
                if (op != NULL)
                {
                    if (!op->defined)
                    {
                        err(RUNTIME_UNITIALIZED_ERR, "Unitialized variable");
                    }

                    if (op->type == K_INTEGER)
                    {
                        op1 = op->data->i;
                    }
                    else if (op->type == K_REAL)
                    {
                        op1 = op->data->d;
                    }
                    else if (op->type == K_STRING)
                    {
                        tmp1 = op->data->string->name;
                    }
                }
                else
                {
                    if (convert_to_k_type(_op1->type) == K_INTEGER)
                    {
                        op1 = _op1->tdata.i;
                    }
                    else if (convert_to_k_type(_op1->type) == K_REAL)
                    {
                        op1 = _op1->tdata.d;
                    }
                    else if (convert_to_k_type(_op1->type) == K_STRING)
                    {
                        tmp1 = _op1->tdata.string->name;
                    }
                }

                op = find_symbol(_op2->str);
                if (op != NULL)
                {
                    if (!op->defined)
                    {
                        err(RUNTIME_UNITIALIZED_ERR, "Unitialized variable");
                    }

                    if (op->type == K_INTEGER)
                    {
                        op2 = op->data->i;
                    }
                    else if (op->type == K_REAL)
                    {
                        op2 = op->data->d;
                    }
                    else if (op->type == K_STRING)
                    {
                        tmp2 = op->data->string->name;
                    }
                }
                else
                {
                    if (convert_to_k_type(_op2->type) == K_INTEGER)
                    {
                        op2 = _op2->tdata.i;
                    }
                    else if (convert_to_k_type(_op2->type) == K_REAL)
                    {
                        op2 = _op2->tdata.d;
                    }
                    else if (convert_to_k_type(_op1->type) == K_STRING)
                    {
                        tmp2 = _op1->tdata.string->name;
                    }
                }

                if (op1 > op2)
                {
                    t->tdata.b = true;
                }
                else
                {
                    t->tdata.b = false;
                }

                if (tmp1 != NULL && tmp2 != NULL)
                {
                    if (strcmp(tmp1, tmp2) > 0)
                    {
                        t->tdata.b = true;
                    }
                    else
                    {
                        t->tdata.b = false;
                    }
                }

                t->type = K_BOOLEAN;

                loop_inst = instruction_list.active;

                break;
            case I_GTE:
                _op1 = (token_t*)instruction_list.active->inst.address1;
                _op2 = (token_t*)instruction_list.active->inst.address2;
                t = (token_t*)instruction_list.active->inst.address3;

                tmp1 = NULL;
                tmp2 = NULL;

                op = find_symbol(_op1->str);
                if (op != NULL)
                {
                    if (!op->defined)
                    {
                        err(RUNTIME_UNITIALIZED_ERR, "Unitialized variable");
                    }

                    if (op->type == K_INTEGER)
                    {
                        op1 = op->data->i;
                    }
                    else if (op->type == K_REAL)
                    {
                        op1 = op->data->d;
                    }
                    else if (op->type == K_STRING)
                    {
                        tmp1 = op->data->string->name;
                    }
                }
                else
                {
                    if (convert_to_k_type(_op1->type) == K_INTEGER)
                    {
                        op1 = _op1->tdata.i;
                    }
                    else if (convert_to_k_type(_op1->type) == K_REAL)
                    {
                        op1 = _op1->tdata.d;
                    }
                    else if (convert_to_k_type(_op1->type) == K_STRING)
                    {
                        tmp1 = _op1->tdata.string->name;
                    }
                }

                op = find_symbol(_op2->str);
                if (op != NULL)
                {
                    if (!op->defined)
                    {
                        err(RUNTIME_UNITIALIZED_ERR, "Unitialized variable");
                    }

                    if (op->type == K_INTEGER)
                    {
                        op2 = op->data->i;
                    }
                    else if (op->type == K_REAL)
                    {
                        op2 = op->data->d;
                    }
                    else if (op->type == K_STRING)
                    {
                        tmp2 = op->data->string->name;
                    }
                }
                else
                {
                    if (convert_to_k_type(_op2->type) == K_INTEGER)
                    {
                        op2 = _op2->tdata.i;
                    }
                    else if (convert_to_k_type(_op2->type) == K_REAL)
                    {
                        op2 = _op2->tdata.d;
                    }
                    else if (convert_to_k_type(_op1->type) == K_STRING)
                    {
                        tmp2 = _op1->tdata.string->name;
                    }
                }

                if (op1 >= op2)
                {
                    t->tdata.b = true;
                }
                else
                {
                    t->tdata.b = false;
                }

                if (tmp1 != NULL && tmp2 != NULL)
                {
                    if (strcmp(tmp1, tmp2) >= 0)
                    {
                        t->tdata.b = true;
                    }
                    else
                    {
                        t->tdata.b = false;
                    }
                }

                t->type = K_BOOLEAN;

                loop_inst = instruction_list.active;

                break;
            case I_EQ:
                _op1 = (token_t*)instruction_list.active->inst.address1;
                _op2 = (token_t*)instruction_list.active->inst.address2;
                t = (token_t*)instruction_list.active->inst.address3;

                tmp1 = NULL;
                tmp2 = NULL;

                op = find_symbol(_op1->str);
                if (op != NULL)
                {
                    if (!op->defined)
                    {
                        err(RUNTIME_UNITIALIZED_ERR, "Unitialized variable");
                    }

                    if (op->type == K_INTEGER)
                    {
                        op1 = op->data->i;
                    }
                    else if (op->type == K_REAL)
                    {
                        op1 = op->data->d;
                    }
                    else if (op->type == K_STRING)
                    {
                        tmp1 = op->data->string->name;
                    }
                }
                else
                {
                    if (convert_to_k_type(_op1->type) == K_INTEGER)
                    {
                        op1 = _op1->tdata.i;
                    }
                    else if (convert_to_k_type(_op1->type) == K_REAL)
                    {
                        op1 = _op1->tdata.d;
                    }
                    else if (convert_to_k_type(_op1->type) == K_STRING)
                    {
                        tmp1 = _op1->tdata.string->name;
                    }
                }

                op = find_symbol(_op2->str);
                if (op != NULL)
                {
                    if (!op->defined)
                    {
                        err(RUNTIME_UNITIALIZED_ERR, "Unitialized variable");
                    }

                    if (op->type == K_INTEGER)
                    {
                        op2 = op->data->i;
                    }
                    else if (op->type == K_REAL)
                    {
                        op2 = op->data->d;
                    }
                    else if (op->type == K_STRING)
                    {
                        tmp2 = op->data->string->name;
                    }
                }
                else
                {
                    if (convert_to_k_type(_op2->type) == K_INTEGER)
                    {
                        op2 = _op2->tdata.i;
                    }
                    else if (convert_to_k_type(_op2->type) == K_REAL)
                    {
                        op2 = _op2->tdata.d;
                    }
                    else if (convert_to_k_type(_op1->type) == K_STRING)
                    {
                        tmp2 = _op1->tdata.string->name;
                    }
                }

                if (op1 == op2)
                {
                    t->tdata.b = true;
                }
                else
                {
                    t->tdata.b = false;
                }

                if (tmp1 != NULL && tmp2 != NULL)
                {
                    if (strcmp(tmp1, tmp2) == 0)
                    {
                        t->tdata.b = true;
                    }
                    else
                    {
                        t->tdata.b = false;
                    }
                }

                t->type = K_BOOLEAN;

                loop_inst = instruction_list.active;

                break;
            case I_NEQ:
                _op1 = (token_t*)instruction_list.active->inst.address1;
                _op2 = (token_t*)instruction_list.active->inst.address2;
                t = (token_t*)instruction_list.active->inst.address3;

                tmp1 = NULL;
                tmp2 = NULL;

                op = find_symbol(_op1->str);
                if (op != NULL)
                {
                    if (!op->defined)
                    {
                        err(RUNTIME_UNITIALIZED_ERR, "Unitialized variable");
                    }

                    if (op->type == K_INTEGER)
                    {
                        op1 = op->data->i;
                    }
                    else if (op->type == K_REAL)
                    {
                        op1 = op->data->d;
                    }
                    else if (op->type == K_STRING)
                    {
                        tmp1 = op->data->string->name;
                    }
                }
                else
                {
                    if (convert_to_k_type(_op1->type) == K_INTEGER)
                    {
                        op1 = _op1->tdata.i;
                    }
                    else if (convert_to_k_type(_op1->type) == K_REAL)
                    {
                        op1 = _op1->tdata.d;
                    }
                    else if (convert_to_k_type(_op1->type) == K_STRING)
                    {
                        tmp1 = _op1->tdata.string->name;
                    }
                }

                op = find_symbol(_op2->str);
                if (op != NULL)
                {
                    if (!op->defined)
                    {
                        err(RUNTIME_UNITIALIZED_ERR, "Unitialized variable");
                    }

                    if (op->type == K_INTEGER)
                    {
                        op2 = op->data->i;
                    }
                    else if (op->type == K_REAL)
                    {
                        op2 = op->data->d;
                    }
                    else if (op->type == K_STRING)
                    {
                        tmp2 = op->data->string->name;
                    }
                }
                else
                {
                    if (convert_to_k_type(_op2->type) == K_INTEGER)
                    {
                        op2 = _op2->tdata.i;
                    }
                    else if (convert_to_k_type(_op2->type) == K_REAL)
                    {
                        op2 = _op2->tdata.d;
                    }
                    else if (convert_to_k_type(_op1->type) == K_STRING)
                    {
                        tmp2 = _op1->tdata.string->name;
                    }
                }

                if (op1 != op2)
                {
                    t->tdata.b = true;
                }
                else
                {
                    t->tdata.b = false;
                }

                if (tmp1 != NULL && tmp2 != NULL)
                {
                    if (strcmp(tmp1, tmp2) < 0)
                    {
                        t->tdata.b = true;
                    }
                    else
                    {
                        t->tdata.b = false;
                    }
                }

                t->type = K_BOOLEAN;

                loop_inst = instruction_list.active;

                break;
            case I_IF:
                t = (token_t*)instruction_list.active->inst.address3;

                if (convert_to_k_type(t->type) != K_BOOLEAN)
                {
                    err(SEMANTIC_TYPE, "wrong type");
                }

                if (t->tdata.b)
                {
                    if_condition = 0;
                    if_condition_state = 0;
                }
                else
                {
                    if_condition = 1;
                }

                if (if_stack1.top != 0 && (if_stack_top(&if_stack1) == 1 || if_stack_top(&if_stack1) == 2))
                {
                    if_condition = 2;
                }

                if_stack_push(&if_stack1, if_condition);

                break;
            case I_ELSE_BEGIN:
                if_condition_state = 1;

                if_stack_pop(&if_stack1, &if_condition);

                break;
            case I_ELSE_END:
                if_condition = 2;
                if_condition_state = 2;
                break;
            case I_CONDITION_LOOP:
                t = (token_t*)instruction_list.active->inst.address3;

                if (t->tdata.b)
                {
                    loop_condition = 0;
                }
                else
                {
                    loop_condition = 1;
                }

                break;
            case I_END_LOOP:
                if (!loop_condition)
                {
                    instruction_list.active = loop_inst;
                    continue;
                }
                else
                {
                    loop_condition = 2;
                }
                break;
            case I_SORT_STRING:
                out = (Symbol_t)instruction_list.active->inst.address1;
                _in = (pair_t *)instruction_list.active->inst.address2;

                p_check_type(_in, ID, K_STRING);

                out->data->string = malloc(sizeof(string_t));

                if (_in[0].token_type == ID)
                {
                    s = find_symbol_wrapper(_in[0].src);
                    s_check_type(s, K_STRING);
                    out->data->string->name = sort(s->data->string->name);
                }
                else
                {
                    out->data->string->name = sort(_in[0].src);
                }

                out->defined = true;

                break;
            case I_LENGTH:
                out = (Symbol_t)instruction_list.active->inst.address1;
                _in = (pair_t *)instruction_list.active->inst.address2;

                if (_in[0].token_type != ID && _in[0].token_type != K_STRING)
                {
                    err(SEMANTIC_TYPE, "wrong type");
                }

                if (_in[0].token_type == ID)
                {
                    s = find_symbol_wrapper(_in[0].src);

                    s_check_type(s, K_STRING);

                    out->data->i = length(s->data->string->name);
                }
                else if (_in[0].token_type == K_STRING)
                {
                    out->data->i = length(_in[0].src);
                }

                out->defined = true;

                break;
            case I_COPY_STRING:
                out = (Symbol_t)instruction_list.active->inst.address1;
                _in = (pair_t *)instruction_list.active->inst.address3;

                p_check_type(&_in[0], ID, K_STRING);
                p_check_type(&_in[1], ID, K_INTEGER);
                p_check_type(&_in[2], ID, K_INTEGER);

                if (_in[0].token_type == ID)
                {
                    s = find_symbol_wrapper(_in[0].src);
                    s_check_type(s, K_STRING);
                    tmp = s->data->string->name;
                }
                else
                {
                    tmp = _in[0].src;
                }

                if (_in[1].token_type == ID)
                {
                    s = find_symbol_wrapper(_in[1].src);
                    s_check_type(s, K_INTEGER);
                    op1 = s->data->i;
                }
                else
                {
                    op1 = strtol(_in[1].src, NULL, 10);
                }

                if (op1 < 1)
                {
                    err(RUNTIME_OTHER_ERR, "u cannot start with number lesser than 1");
                }

                if (_in[2].token_type == ID)
                {
                    s = find_symbol_wrapper(_in[2].src);
                    s_check_type(s, K_INTEGER);
                    op2 = s->data->i;
                }
                else
                {
                    op2 = strtol(_in[2].src, NULL, 10);
                }

                out->data->string = malloc(sizeof(string_t));
                out->data->string->name = copy(tmp, (int)op1, (int)op2);
                out->defined = true;

                break;
            case I_FIND_STRING:
                out = (Symbol_t)instruction_list.active->inst.address1;
                _in = (pair_t *)instruction_list.active->inst.address3;

                p_check_type(&_in[0], ID, K_STRING);
                p_check_type(&_in[1], ID, K_STRING);

                if (_in[0].token_type == ID)
                {
                    s = find_symbol_wrapper(_in[0].src);
                    s_check_type(s, K_STRING);
                    tmp1 = s->data->string->name;
                }
                else
                {
                    tmp1 = _in[0].src;
                }

                if (_in[1].token_type == ID)
                {
                    s = find_symbol_wrapper(_in[1].src);
                    s_check_type(s, K_STRING);
                    tmp2 = s->data->string->name;
                }
                else
                {
                    tmp2 = _in[1].src;
                }

                out->data->i = find(tmp1, tmp2);
                out->defined = true;

                break;
            case I_CONCATENATE:
                _op1 = (token_t *)instruction_list.active->inst.address1;
                _op2 = (token_t *)instruction_list.active->inst.address2;
                t = (token_t *)instruction_list.active->inst.address3;

                op = find_symbol(_op1->str);
                if (op != NULL)
                {
                    if (!op->defined)
                    {
                        err(RUNTIME_UNITIALIZED_ERR, "Unitialized variable");
                    }

                    tmp1 = op->data->string->name;
                }
                else
                {
                    tmp1 = _op1->tdata.string->name;
                }

                op = find_symbol(_op2->str);
                if (op != NULL)
                {
                    if (!op->defined)
                    {
                        err(RUNTIME_UNITIALIZED_ERR, "Unitialized variable");
                    }

                    tmp2 = op->data->string->name;
                }
                else
                {
                    tmp2 = _op2->tdata.string->name;
                }

                t->tdata.string = malloc(sizeof(string_t));
                t->tdata.string->name = malloc(strlen(tmp1) + strlen(tmp2) + 1);
                strcpy(t->tdata.string->name, tmp1);
                strcat(t->tdata.string->name, tmp2);

                break;
            case I_STOP:
                return 0;
            default:
                break;
        }

        list_next();
    }
}
