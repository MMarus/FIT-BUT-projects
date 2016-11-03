/* ************************* parser.c ************************* */
/* Kodovani:		UTF-8					                    */
/* Predmet:		Formalni jazyky a prekladace (IFJ)	            */
/* Projekt:		Implementace interpretu imperativniho	        */
/*			    jazyka IFJ14				                    */
/* Varianta zadani:	a/2/I					                    */
/* Autori:		Fáber Jakub		xfaber02	                    */
/*			Hladík Daniel		xhladi21	                    */
/*			Marušič Marek		xmarus05	                    */
/*			Surovčík Tomáš		xsurov04	                    */
/* ************************************************************ */

#include <strings.h>
#include "parser.h"

token_t *result;

void eval() /////// CALL SEMANTICS
{
    sa_core();
}

void write_fun()
{
    getNextToken(&token);
    if (token.type != L_BRACKET)
        err(SYNTAX_ERR, "Expecting (");

    getNextToken(&token); // 1st parameter

    if (token.type == R_BRACKET)
        err(SYNTAX_ERR, "Expecting at least 1 parameter to write()");

    value_u *u;
    bool comma = true; // for (,)
    bool need_comma = false;
    int val_type = -1;
    symbol_t *s = NULL;
    while (token.type != R_BRACKET) {

        if (token.type == COMMA) {
            if (comma == false) {
                comma = true;
                need_comma = false;
                goto skip_analyzing;
            }
            else
                err(SYNTAX_ERR, "Unexpected comma");
        } else
            comma = false;

        if (token.type == ID) {
            if (need_comma == true)
                err(SYNTAX_ERR, "Expecting , in write call");

            s = find_symbol(token.str);
            if (!s)
                err(SEMANTIC_DEFINITION_ERR, "Variable not found in symbol table");

            val_type = s->type;
            u = s->data;
            need_comma = true;
        } else {
            if (need_comma == true)
                err(SYNTAX_ERR, "Expecting , in write call");

            need_comma = true;

            u = init_value_t();
            if (token.type == STR || token.type == K_TRUE || token.type == K_FALSE) {
                val_type = K_STRING;
                u->string = make_string(token.str);
            }
            else if (token.type == INT) {
                val_type = K_INTEGER;
                u->i = atoi(token.str->name);
            }
            else if (token.type == DOUBLE) {
                val_type = K_REAL;
                u->d = atof(token.str->name);
            } else {
                err(SYNTAX_ERR, "Unexpected token as parameter");
            }
        }

        add_instruction(I_WRITE, s, (void *) val_type, u);

#ifdef DEBUG
//        printf("Processing write parameter | type: %d", val_type);
#endif

        skip_analyzing:
        getNextToken(&token);
    }

    if (comma == true)
        err(SYNTAX_ERR, "Unexpected ,");

    getNextToken(&token); // eats ; OR end

}

void readln_fun()
{
    getNextToken(&token);
    if (token.type != L_BRACKET)
        err(SYNTAX_ERR, "Expecting (");

    getNextToken(&token);
    if (token.type != ID)
        err(SYNTAX_ERR, "Expected identifier");

    Symbol_t s = find_symbol(token.str);
    if (!s)
        err(SEMANTIC_DEFINITION_ERR, "Variable not found in symbol table");

    if (token.type == K_BOOLEAN)
        err(SEMANTIC_TYPE, "Invalid type (boolean) in readln()");

    add_instruction(I_READLN, NULL, NULL, s);

    getNextToken(&token);
    if (token.type != R_BRACKET)
        err(SYNTAX_ERR, "Expecting )");

    getNextToken(&token); // eats ; OR end
}

void while_begin()
{
    result = init_token2();
    getNextToken(&token);
    eval();
    add_instruction(I_CONDITION_LOOP, NULL, NULL, result);

    if (token.type != K_DO)
        err(SYNTAX_ERR, "Expecting do");

    getNextToken(&token);
    if (token.type != K_BEGIN)
        err(SYNTAX_ERR, "Expecting begin");

    subbody(1, false, true);

    add_instruction(I_END_LOOP, NULL, NULL, NULL);
}

token_t *init_token2()
{
    token_t *t = malloc(sizeof(token));
    if (!t)
        err(INTERNAL_ERR, "Error allocating in init_token2");
    return t;
}

item_inst_t *function(ftable_item_t *f, token_t *res)
{
    item_inst_t *return_add = add_instruction(I_CALL, NULL, &f->item, instruction_list.last);

    getNextToken(&token);
    if (token.type != L_BRACKET)
        err(SYNTAX_ERR, "Expecting (");

    value_u *u;

    bool comma = true; // for (,)
    int val_type = -1;

    int id_index = 0;
    int type_index = 0;
    int params = 0;

    getNextToken(&token);
    while (token.type != R_BRACKET) {
        if (token.type == COMMA) {
            if (comma == false) {
                comma = true;
                goto skip_params;
            }
            else
                err(SYNTAX_ERR, "Unexpected comma");
        } else
            comma = false;

        result = init_token2();

        if (token.type == ID) {
            symbol_t *s = find_symbol(token.str);
            if (!s)
                err(SEMANTIC_DEFINITION_ERR, "Variable not found in symbol table");

            val_type = s->type;
            u = s->data;
        } else {
            u = init_value_t();
            if (token.type == STR) {
                val_type = K_STRING;
                u->string = make_string(token.str);
            } else if (token.type == INT) {
                val_type = K_INTEGER;
                u->i = atoi(token.str->name);
            } else if (token.type == DOUBLE) {
                val_type = K_REAL;
                u->d = atof(token.str->name);
            } else if (token.type == K_TRUE || token.type == K_FALSE) {
                val_type = K_BOOLEAN;
                u->b = (token.type == K_TRUE) ? true : false;
            } else {
                err(SYNTAX_ERR, "Unexpected token as a parameter");
            }
        }

        Symbol_t s = compare_functions_calls(f, &id_index, &type_index, val_type);
        if (!s)
            err(SEMANTIC_TYPE, "Invalid parameter type in function call");


        ++params;

        // add to symbol table
        add_instruction(I_PARAM_ADD, s, (void *) val_type, u);

        skip_params:
        getNextToken(&token);
    }

    if (comma == true) {
        if (!(f->item.params_cmp_string.ids_len == 0 && params == 0))
            err(SEMANTIC_TYPE, "Invalid function signature");

        if (f->item.params_cmp_string.ids_len != 0)
            err(SYNTAX_ERR, "Unexpected ,");
    }

    result->type = f->item.return_value.type; // remove after
    getNextToken(&token);

    return return_add;
}

Symbol_t compare_functions_calls(ftable_item_t *f, int *type_index, int *id_index, int val_type)
{
    char *iter_types = &f->item.params_cmp_string.types[*type_index];
    int defined_type = get_type(iter_types[0]);
    (*type_index)++;

    if (defined_type != val_type)
        return NULL;

    Symbol_t s = get_uval(&f->item, id_index);
    s->defined = true;

    return s;
}

Symbol_t get_uval(function_env_t *f, int *index)
{
    char tmp[DEFAULT_MEM_ALLOC] = {0,};
    char *iter_ids = &f->params_cmp_string.ids[*index];
    int i = 0;

    while (iter_ids[i] != '\0') {
        if (iter_ids[i] != '|')
            tmp[i] = iter_ids[i];
        else {
//            *index = i;
            break;
        }
        ++i;
    }
    tmp[i] = '\0';
    Symbol_t s = find_symbol_wrapper(tmp);
    *index += ++i; // skip |
    return s;
}

inline int get_type(char id)
{
    if (id == 'i')
        return K_INTEGER;
    else if (id == 's')
        return K_STRING;
    else if (id == 'r')
        return K_REAL;
    else if (id == 'b')
        return K_BOOLEAN;

    return -1;
}


void assignment()
{
    ftable_item_t *tmp;
    Symbol_t s;
    result = init_token2();
    token_t *ret_tok;
    int left_type;
    bool ret = false;

    if ((s = find_symbol(token.str)) == NULL) {
        if (cmp_string(tab.active->item.str, token.str) != 0)
            err(SEMANTIC_DEFINITION_ERR, "Accessing undeclared variable");
        else
            ret = true;
    }

    left_type = s->type;

    getNextToken(&token);
    if (token.type != ASSIGN)
        err(SYNTAX_ERR, "Missing :=");

    item_inst_t *return_add = NULL;
    getNextToken(&token);
    if ((tmp = find_function(token.str)) != NULL) {
        ret_tok = result;
        return_add = function(tmp, result);
        result = ret_tok;
    }
    else {
        if (token.type == K_SORT) {
            sort_fun(s);
            return;
        } else if (token.type == K_LENGTH) {
            length_fun(s);
            return;
        } else if (token.type == K_COPY) {
            copy_fun(s);
            return;
        } else if (token.type == K_FIND) {
            find_fun(s);
            return;
        }

        eval(); // SA evals and modifies result on address result

//        if (convert_to_k_type(result->type) != left_type)
//            err(SEMANTIC_TYPE, "Incompatible types", token.lineNumber);
    }

    add_instruction(I_ASSIGN, s, NULL, result);
    if (ret)
        add_instruction(I_RET, NULL, return_add, result);
}

pair_t *init_pair(int n)
{
    pair_t *pair = malloc(sizeof(pair_t) * n);
}

void copy_fun(Symbol_t in)
{
    if (in->type != K_STRING)
        err(SEMANTIC_TYPE, "Copy returns string");

    getNextToken(&token);
    if (token.type != L_BRACKET)
        err(SYNTAX_ERR, "Expecting (");

    getNextToken(&token);

    pair_t *pair = init_pair(3);

    pair[0].src = copy_string(token.str->name, token.str->length);
    pair[0].token_type = convert_to_k_type(token.type);

    getNextToken(&token);
    if (token.type != COMMA)
        err(SYNTAX_ERR, "Expecting , in copy()");

    getNextToken(&token);
    pair[1].src = copy_string(token.str->name, token.str->length);
    pair[1].token_type = convert_to_k_type(token.type);

    getNextToken(&token);
    if (token.type != COMMA)
        err(SYNTAX_ERR, "Expecting , in copy()");

    getNextToken(&token);
    pair[2].src = copy_string(token.str->name, token.str->length);
    pair[2].token_type = convert_to_k_type(token.type);

    add_instruction(I_COPY_STRING, in, NULL, pair);// 0 src, 1 start, 2 end

    getNextToken(&token);
    if (token.type != R_BRACKET)
        err(SYNTAX_ERR, "Expecting )");

    getNextToken(&token);

}

void find_fun(Symbol_t in)
{
    if (in->type != K_INTEGER)
        err(SEMANTIC_TYPE, "Find returns integer");

    getNextToken(&token);
    if (token.type != L_BRACKET)
        err(SYNTAX_ERR, "Expecting (");

    pair_t *pair = init_pair(2);

    getNextToken(&token);

    pair[0].src = copy_string(token.str->name, token.str->length + 1);
    pair[0].token_type = convert_to_k_type(token.type);

    getNextToken(&token);
    if (token.type != COMMA)
        err(SYNTAX_ERR, "Expecting , in find()");

    getNextToken(&token);
    pair[1].src = copy_string(token.str->name, token.str->length + 1);
    pair[1].token_type = convert_to_k_type(token.type);

    add_instruction(I_FIND_STRING, in, NULL, pair);

    getNextToken(&token);
    if (token.type != R_BRACKET)
        err(SYNTAX_ERR, "Expecting )");

    getNextToken(&token);
}

void length_fun(Symbol_t in)
{
    if (in->type != K_INTEGER)
        err(SEMANTIC_TYPE, "Length returns integer");

    getNextToken(&token);
    if (token.type != L_BRACKET)
        err(SYNTAX_ERR, "Expecting (");

    pair_t *pair = init_pair(1);

    getNextToken(&token);
    pair[0].src = copy_string(token.str->name, token.str->length + 1);
    pair[0].token_type = convert_to_k_type(token.type);

    add_instruction(I_LENGTH, in, pair, NULL);

    getNextToken(&token);
    if (token.type != R_BRACKET)
        err(SYNTAX_ERR, "Expecting )");

    getNextToken(&token);
}

void sort_fun(Symbol_t out)
{
    if (out->type != K_STRING)
        err(SEMANTIC_TYPE, "Sort returns string");

    getNextToken(&token);
    if (token.type != L_BRACKET)
        err(SYNTAX_ERR, "Expecting (");

    getNextToken(&token);
    pair_t *pair = init_pair(1);

    pair[0].src = copy_string(token.str->name, token.str->length + 1);
    pair[0].token_type = convert_to_k_type(token.type);

    add_instruction(I_SORT_STRING, out, pair, NULL);

    getNextToken(&token);
    if (token.type != R_BRACKET)
        err(SYNTAX_ERR, "Expecting )");

    getNextToken(&token);
}


void if_begin()
{
    result = init_token2();
    getNextToken(&token);
    eval(); // SA evals and modifies result on address result

    add_instruction(I_IF, NULL, NULL, result);

    if (token.type != K_THEN)
        err(SYNTAX_ERR, "Expecting then");

    getNextToken(&token);
    if (token.type != K_BEGIN)
        err(SYNTAX_ERR, "Expecting begin");

    subbody(1, false, true);
    if (token.type != K_ELSE)
        err(SYNTAX_ERR, "Expecting else");

    add_instruction(I_ELSE_BEGIN, NULL, NULL, NULL);

    getNextToken(&token);
    if (token.type != K_BEGIN)
        err(SYNTAX_ERR, "Expecting begin");
    subbody(1, false, true);

    add_instruction(I_ELSE_END, NULL, NULL, NULL);
}

//////////////////////////////////////////////////////////////


void statement()
{
#ifdef DEBUG2
    printf("\tENTERING statement()\n");
#endif
    if (token.type == SEMICOLON)
        return;
    else if (token.type == ID)
        assignment();
    else if (token.type == K_IF)
        if_begin();
    else if (token.type == K_WHILE)
        while_begin();
    else if (token.type == K_WRITE)
        write_fun();
    else if (token.type == K_REDALN)
        readln_fun();

#ifdef DEBUG2
    printf("\tEXITING statement()\n");
#endif
}

void parse()
{
#ifdef DEBUG
    printf("\tENTERING parse()\n");
#endif
    activate_global_env();

    getNextToken(&token);
    if (token.type == K_VAR)
        var_decl(true);

    if (token.type == K_FUNCTION)
        fun_decl();

    if (all_defined() != NULL)
        err(SEMANTIC_DEFINITION_ERR, "Found undefined functions");

#ifdef DEBUG
    printf("\n\tENTERING !!!!!!!!!!!!!!!!!!!! MAIN !!!!!!!!!!!!!!!!!!!!\n\n");
#endif
    activate_global_env();
    // main() is trigger
    instruction_list.active = add_instruction(I_NOP, NULL, NULL, NULL);

    subbody(1, false, true);

    if (token.type != DOT)
        err(SYNTAX_ERR, "Expecting .");

    getNextToken(&token);
    if (token.type != END_OF_FILE)
        err(SYNTAX_ERR, "Expecting end of file .");

    add_instruction(I_STOP, NULL, NULL, NULL);
#ifdef DEBUG
    printf("EXITING parse()\n");
#endif
}


int subbody(int begin_end, bool more, bool can_begin)
{
    getNextToken(&token);

    if (token.type == END_OF_FILE)
        err(SYNTAX_ERR, "Unmatched begin/end");

    if (token.type == K_BEGIN) {
        if (can_begin == true)
            return subbody(begin_end + 1, false, true);
        else
            err(SYNTAX_ERR, "Invalid character ; (a)");
    } else if (token.type == K_END) {
        if (begin_end == 0 && more == false)
            return 0;

        if (more == false)
            return subbody(begin_end - 1, false, false);
        else
            err(SYNTAX_ERR, "Invalid character ; (b)");
    } else {
        if ((more == true && token.type == SEMICOLON))
            err(SYNTAX_ERR, "Invalid character ; (c)");

        if (begin_end == 0 && more == false)
            return 0;
        else if (token.type == SEMICOLON) {
            return subbody(begin_end, true, true);
        }

        if (can_begin == true)
            statement();
        else
            err(SYNTAX_ERR, "Invalid character ; (d)");

        if (!(token.type == K_END || token.type == SEMICOLON))
            err(SYNTAX_ERR, "Expected ;");

        if (token.type == K_END) {
            more = false;
            --begin_end;
        } else if (token.type == SEMICOLON) {
            more = true;
            can_begin = true;
        }
    }

    return subbody(begin_end, more, can_begin);

}

void var_decl(bool def_needed)
{
#ifdef DEBUG
    printf("\tENTERING var_decl()\n");
#endif

    getNextToken(&token);
    if (token.type != ID && def_needed == true)
        err(SYNTAX_ERR, "Expecting variable declaration");

    if (token.type != ID)
        return;

    token_t tmp_token;
    prepare_token(&tmp_token);
    tmp_token.str = make_string(token.str);
    tmp_token.lineNumber = token.lineNumber;

    if (find_function(token.str) != NULL)
        err(SEMANTIC_DEFINITION_ERR, "Can't have same name for function and variable");

    getNextToken(&token);
    if (token.type != COLON)
        err(SYNTAX_ERR, "Expected :");

    getNextToken(&token);
    if (!(token.type == K_INTEGER || token.type == K_BOOLEAN
            || token.type == K_REAL || token.type == K_STRING))
        err(SYNTAX_ERR, "Not a valid data type");

    tmp_token.type = token.type;

    getNextToken(&token);
    if (token.type != SEMICOLON)
        err(SYNTAX_ERR, "Expected ;");

    if (add_to_stable(&tmp_token) == NULL)
        err(SEMANTIC_DEFINITION_ERR, "Variable redefination error");

    var_decl(false);

#ifdef DEBUG
    printf("\tEXITING var_decl()\n");
#endif
}

void fun_decl()
{
#ifdef DEBUG
    printf("\tENTERING fun_decl()\n");
#endif

    if (token.type != K_FUNCTION)
        return;

    getNextToken(&token);
    if (token.type != ID)
        err(SYNTAX_ERR, "Expecting function name");

    function_env_t *f_env = activate_function_env(token.str);
    function_env_t *f_env_copy = NULL;

    if (token.type != ID)
        err(SYNTAX_ERR, "Expecting function name");

    // checks if functions is already in a function table
    bool in_table = false;
    if (f_env) {
        in_table = true;
        f_env_copy = function_delete(f_env);
    }
    f_env = function_append(token.str);

    getNextToken(&token);
    if (token.type != L_BRACKET)
        err(SYNTAX_ERR, "Expecting (");

    // empty parameter list
    getNextToken(&token);
    if (token.type == R_BRACKET)
        goto skip_params;

    // we have some parameters
    if (token.type != ID)
        err(SYNTAX_ERR, "Expecting parameter identifier/empty list");
    parse_function_params(f_env, false, false);

    skip_params:
    parse_function_ret(f_env);

    if (in_table == true) {
        if (compare_functions(f_env, f_env_copy, &token) != 0)
            err(SEMANTIC_DEFINITION_ERR, "Different function signature");
        function_free(f_env_copy);
    }

    if (parse_function_forward(f_env) == DEFINED) {
        if (token.type == K_VAR)
            var_decl(true);

        fun_body();
        getNextToken(&token);
    }

    fun_decl();
#ifdef DEBUG
    printf("\tEXITING fun_decl()\n");
#endif
}

void fun_body()
{
    if (token.type != K_BEGIN)
        err(SYNTAX_ERR, "Expected begin");

    tab.active->item.address = add_instruction(I_FUN_START, NULL, NULL, NULL);

    subbody(1, false, true);

    if (token.type != SEMICOLON)
        err(SYNTAX_ERR, "Expected ; at the end of function definition");
}

int parse_function_forward(function_env_t *f)
{
#ifdef DEBUG
    printf("\t\tENTERING parse_function_forward()\n");
#endif
    getNextToken(&token);
    if (token.type == K_BEGIN || token.type == K_VAR) {
        f->defined = true;
        f->declared = true;
        // printf("DEF -> DEF: %d | DEC: %d\n", f->defined, f->declared);

        return DEFINED;

    } else if (token.type == K_FORWARD) {
        if (f->declared || f->defined)
            err(SEMANTIC_DEFINITION_ERR, "Multiple function definition/declaration");
        else
            f->declared = true;

        getNextToken(&token);
        if (token.type != SEMICOLON)
            err(SYNTAX_ERR, "Expecting ; after forward");

        // printf("DEC -> DEF: %d | DEC: %d\n", f->defined, f->declared);
        getNextToken(&token);

        return DECLARED;
    }
}

void parse_function_params(function_env_t *f, bool semicolon, bool in_table)
{
#ifdef DEBUG
    printf("\tENTERING parse_function_params()\n");
#endif

    token_t tmp_token;
    prepare_token(&tmp_token);

    if (token.type == ID) {
        tmp_token.str = make_string(token.str);

        add_id_param_cmp(f, &tmp_token);

        getNextToken(&token);
        if (token.type != COLON)
            err(SYNTAX_ERR, "Expected :");

        getNextToken(&token);
        if (!(token.type == K_INTEGER || token.type == K_BOOLEAN
                || token.type == K_REAL || token.type == K_STRING))
            err(SYNTAX_ERR, "Not a valid data type");

        tmp_token.type = token.type;
        add_type_param_cmp(f, &token);

        getNextToken(&token);

        if (token.type == SEMICOLON) {
            getNextToken(&token);
            semicolon = true;
        } else
            semicolon = false;

        if (add_to_stable(&tmp_token) == NULL)
            err(SEMANTIC_DEFINITION_ERR, "Redefinination of variable");

        parse_function_params(f, semicolon, false);

    } else if (token.type == R_BRACKET && semicolon == false) {
        return;
    } else
        err(SYNTAX_ERR, "Expecting parameter identifier");

#ifdef DEBUG
    printf("\tEXITING parse_function_params()\n");
#endif
}

void parse_function_ret(function_env_t *f)
{
#ifdef DEBUG
    printf("\tENTERING parse_function_ret()\n");
#endif

    getNextToken(&token);
    if (token.type != COLON)
        err(SYNTAX_ERR, "Expecting : before return type");

    getNextToken(&token);
    if (!(token.type == K_INTEGER || token.type == K_BOOLEAN
            || token.type == K_REAL || token.type == K_STRING))
        err(SYNTAX_ERR, "Expecting return value data type");

    f->return_value.type = token.type;

    getNextToken(&token);
    if (token.type != SEMICOLON)
        err(SYNTAX_ERR, "Expecting ; after function var_decl");

#ifdef DEBUG
    printf("\tEXITING parse_function_ret()\n");
#endif
}
