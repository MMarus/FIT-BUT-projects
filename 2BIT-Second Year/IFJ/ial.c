#include "ial.h"
#include "parser.h"

char *copy(char *string, int start, int length)
{
    char *tmp = malloc(sizeof(char) * length + 1);

    strncpy(tmp, &string[start - 1], (size_t)length);
    tmp[length] = '\0';

    return tmp;
}

int find(char *string, char *search)
{
    if(strlen(search) == 0){return (1);}

    int search_length = strlen(search);
    int string_length = strlen(string);
    int *overlap = malloc(sizeof(int) * search_length);

    int a = -1;
    overlap[0] = a;
    for (int i = 1; i < search_length; i++) {
        while (a > -1 && search[a+1] != search[i]) {
            a = overlap[a];
        }

        if (search[i] == overlap[a+1]) {
            a++;
        }

        overlap[i] = a;
    }

    int k = -1;
    for (int i = 0; i < string_length; i++) {
        while (k > -1 && search[k+1] != string[i]) {
            k = overlap[k];
        }

        if (string[i] == search[k+1]) {
            k++;
        }

        if (k == search_length-1)
        {
            free(overlap);
            return (i-k+1);
        }
    }

    free(overlap);
    return (0);
}

int length(char *string)
{
    return (strlen(string));
}

/* HEAP SORT - START */

void _swap(char *a, char *b)
{
    char tmp = *a;
    *a = *b;
    *b = tmp;
}

void siftDown(char *arr, int root, int length)
{
    while ((root * 2 + 1) < length) {
        int swap = root;
        int child_l = root * 2 + 1;
        int child_r = root * 2 + 2;

        if (arr[child_l] > arr[root]) {
            swap = child_l;
        }

        if (child_r < length && arr[swap] < arr[child_r]) {
            swap = child_r;
        }

        if (swap == root) {
            return;
        }
        else {
            _swap(&arr[root], &arr[swap]);
            root = swap;
        }
    }
}

char *sort(char *string)
{
    int length = strlen(string);
    char *arr = malloc((length+1) * sizeof(char));
    strcpy(arr, string);

    for (int i = length / 2 - 1; i >= 0; i--) {
        siftDown(arr, i, length);
    }

    for (int i = length - 1; i > 0; i--) {
        _swap(&arr[i], &arr[0]);
        siftDown(arr, 0, i);
    }

    return arr;
}

/* HEAP SORT - END */


void ftable_init()
{
    init_token();

    // initialize global enviroment
    ftable_item_t *global_scope = malloc(sizeof(ftable_item_t));
    global_scope->item.params_cmp_string.ids_len = 0;
    global_scope->item.str = NULL;
    global_scope->next = NULL;

    tree_init(&global_scope->item.env_root);

    tab.active = global_scope;
    tab.first = global_scope;
    tab.last = global_scope;
}

void function_free(function_env_t *f)
{
    free_string(f->str);

    free(f->params_cmp_string.ids);
    free(f->params_cmp_string.types);

    // free all nodes in function enviroment tree
    tree_free(f->env_root);

    // free env struct
    free(f);
}

void ftable_free()
{
    free_token();

    ftable_item_t *iter_tmp = tab.first;
    ftable_item_t *to_free;

    while (iter_tmp) {

#ifdef DEBUG
        if (iter_tmp->item.str)
            printf("%s\n", iter_tmp->item.str->name);
        else
            printf("%s\n", "GLOBAL");
#endif
        to_free = iter_tmp;
        iter_tmp = iter_tmp->next;

        function_free(&to_free->item);
    }
}

// activates global scope as an active environment
inline void activate_global_env()
{
    tab.active = tab.last;
}

// searches for function according its name and sets it as an active environment
function_env_t *activate_function_env(string_t *f_name)
{
    ftable_item_t *f = find_function(f_name);

#ifdef DEBUG
    printf("activate_function_env() | %p\n", f);
#endif

    if (f != NULL) {
        tab.active = f;
        return &f->item;
    } else {
//        err(SYNTAX_ERR, "Can't find function | :o", NO_LINE);
        return NULL;
    }
}

function_env_t *function_append(string_t *function_name)
{
    ftable_item_t *new_f = malloc(sizeof(ftable_item_t));
    init_param_cmp(&new_f->item);
    new_f->item.declared = false;
    new_f->item.defined = false;
    new_f->item.str = make_string(function_name);
    new_f->next = tab.first;
//    new_f->item.params_cmp_string.types = NULL;

    tree_init(&new_f->item.env_root);

    tab.active = new_f;
    tab.first = new_f;

    return &new_f->item;
}

int compare_functions(function_env_t *f_tab, function_env_t *f_copy, token_t *token)
{
    assert(f_tab);
    assert(f_copy);

    f_tab->defined = f_copy->defined;
    f_tab->declared = f_copy->declared;

    int ids = strcmp(f_tab->params_cmp_string.ids, f_copy->params_cmp_string.ids);
    int types = strcmp(f_tab->params_cmp_string.types, f_copy->params_cmp_string.types);

    // -1 if different signature
    if (ids || types)
        return -1;
    else
        return 0;
}

function_env_t *all_defined()
{
    ftable_item_t *iter_tmp = tab.first;

    while (iter_tmp != tab.last) {
        if (iter_tmp->item.defined != true)
            return &iter_tmp->item;

#ifdef DEBUG
        printf("all_defined() - f_name:%s - ids:%s - types:%s\n", iter_tmp->item.str->name, iter_tmp->item.params_cmp_string.ids, iter_tmp->item.params_cmp_string.types);
#endif

        iter_tmp = iter_tmp->next;
    }

    return NULL;
}

//int compare_functions_call(function_env_t *f, )
//{
//
//}

function_env_t *function_delete(function_env_t *f)
{
    ftable_item_t *iter = tab.first;
    ftable_item_t *prev = tab.first;

    while (iter != tab.last) {
        if (f == &iter->item) {
            if (&tab.first->item != f)
                prev->next = iter->next;
            else
                tab.first = iter->next;

            if (&tab.active->item == f)
                tab.active = tab.first;
            return f;
        }
        prev = iter;
        iter = iter->next;
    }

    return NULL;
}

//////////////////////////////////////////////////////////////////////

Symbol_t find_symbol_wrapper(char *symbol_name)
{
    string_t s;
    s.length = -1;
    s.name = symbol_name;
    return find_symbol(&s);
}

Symbol_t find_symbol(string_t *symbol_name)
{
    Symbol_t tmp;
    // find in active function environment
    if (tab.active != tab.last) {
        if ((tmp = find_in_tree(tab.active->item.env_root, symbol_name)) != NULL)
            return tmp;
    }

    // find in global scope
    return find_in_tree(tab.last->item.env_root, symbol_name);
}

ftable_item_t *find_function(string_t *function_name)
{
    ftable_item_t *iter_tmp = tab.first;

    while (iter_tmp != tab.last) {
//        printf("P: %s | T: %s\n", function_name->name, iter_tmp->item.str->name);
        if (strcmp(function_name->name, iter_tmp->item.str->name) == 0)
            return iter_tmp;

        iter_tmp = iter_tmp->next;
    }

    return NULL;
}


symbol_t *init_symbol_t()
{
    symbol_t *s = malloc(sizeof(symbol_t));
    if (!s) {
        err(INTERNAL_ERR, "Error allocating in init_symbol_t()");
        return NULL;
    }

    s->str = NULL;
    s->defined = false;
    s->left = NULL;
    s->right = NULL;
    s->data = init_value_t();
}

int convert_to_k_type(int type)
{
    if (type == INT)
        return K_INTEGER;
    else if (type == DOUBLE)
        return K_REAL;
    else if (type == K_FALSE || type == K_TRUE)
        return K_BOOLEAN;
    else if (type == STR)
        return K_STRING;
    else if (type == ID)
        return ID;

    if (type == K_INTEGER || type == K_BOOLEAN
            || type == K_STRING || type == K_REAL)
        return type;

    return -1;
}

// returns NULL if the table contains same symbol name
Symbol_t add_to_stable(token_t *t)
{
#ifdef DEBUG
    printf("add_to_stable()| id: %s | type: %d | env: %s\n", t->str->name, t->type, (tab.active == tab.last) ? "GLOBAL" : tab.active->item.str->name);
#endif
    return add_to_tree(&tab.active->item.env_root, t);
}

//////////////////////////////////////////////////////////////////////

Symbol_t add_to_tree(Symbol_t *r, token_t *t)
{
    if (*r == NULL) {
        *r = malloc(sizeof(struct symbol_s));

        (*r)->defined = false;
        (*r)->type = t->type;
        (*r)->data = init_value_t();
        (*r)->str = make_string(t->str);
        (*r)->left = NULL;
        (*r)->right = NULL;

        return *r;
    }

    Symbol_t *bi;
    if (strcmp(t->str->name, (*r)->str->name) < 0) {
        bi = &(*r)->left;
    } else if (strcmp(t->str->name, (*r)->str->name) > 0) {
        bi = &(*r)->right;
    } else {
        return NULL;
    }

    return add_to_tree(bi, t);
}

Symbol_t find_in_tree(Symbol_t r, string_t *str)
{
    if (r == NULL)
        return NULL;

    Symbol_t bi;
    if (strcmp(str->name, r->str->name) < 0) {
        bi = r->left;
    } else if (strcmp(str->name, r->str->name) > 0) {
        bi = r->right;
    } else {
        return r;
    }

    return find_in_tree(bi, str);
}

void tree_init(Symbol_t *r)
{
    *r = NULL;
}

void tree_free(Symbol_t r)
{
    if (r == NULL)
        return;

    tree_free(r->left);
    tree_free(r->right);

    free_string(r->str);
    free(r);
}

void dbg_print_tree(Symbol_t r)
{
    if (r == NULL)
        return;

    dbg_print_tree(r->left);

    printf("%s %d\n", r->str->name, r->type);

    dbg_print_tree(r->right);
}

void init_param_cmp(function_env_t *f)
{
    f->params_cmp_string.ids = malloc(DEFAULT_MEM_ALLOC);
    f->params_cmp_string.ids_len = 0;
    f->params_cmp_string.allocated_ids = DEFAULT_MEM_ALLOC;

    f->params_cmp_string.types = malloc(DEFAULT_MEM_ALLOC);
    f->params_cmp_string.ids_len = 0;
    f->params_cmp_string.allocated_types = DEFAULT_MEM_ALLOC;
}

void add_id_param_cmp(function_env_t *f, token_t *t)
{
    assert(f);
    assert(t);

    if (t->str->length + f->params_cmp_string.ids_len >= f->params_cmp_string.allocated_ids) {
        f->params_cmp_string.ids = realloc(f->params_cmp_string.ids, DEFAULT_MEM_ALLOC + f->params_cmp_string.allocated_ids);
        f->params_cmp_string.allocated_ids = DEFAULT_MEM_ALLOC + f->params_cmp_string.allocated_ids;
    }

    strcat(f->params_cmp_string.ids, t->str->name);
    strcat(f->params_cmp_string.ids, "|");
    f->params_cmp_string.ids_len += t->str->length;
}

void add_type_param_cmp(function_env_t *f, token_t *t)
{
    assert(f);
    assert(t);

    if (f->params_cmp_string.types_len + 1 >= f->params_cmp_string.allocated_types) {
        f->params_cmp_string.types = realloc(f->params_cmp_string.types, DEFAULT_MEM_ALLOC + f->params_cmp_string.allocated_types);
        f->params_cmp_string.allocated_types = DEFAULT_MEM_ALLOC + f->params_cmp_string.allocated_types;
    }

    if (token.type == K_INTEGER)
        strcat(f->params_cmp_string.types, "i");
    else if (token.type == K_BOOLEAN)
        strcat(f->params_cmp_string.types, "b");
    else if (token.type == K_STRING)
        strcat(f->params_cmp_string.types, "s");
    else if (token.type == K_REAL)
        strcat(f->params_cmp_string.types, "r");

    f->params_cmp_string.types_len += 1;
}

value_u *init_value_t()
{
    value_u *v = malloc(sizeof(value_u));

    if (!v)
        err(INTERNAL_ERR, "Error allocating in init_value_t()");

    return v;
}
