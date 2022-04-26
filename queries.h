#include "general_defs.h"
#include "general_functions.h"
#include "validation.h"
#include "data_structure_build.h"
#include "sorting.h"

#define PRINT_INVALID_CMD(EXPLAIN) \
    fprintf(stderr, "error. invalid command. (%s) please try again.\n", EXPLAIN);

#define QUIERIES_HELP_TXT                                                                                \
    "commands and usage:\n"                                                                              \
    "select <FIELD NAME> <OPERATOR> <VALUE>\n"                                                           \
    "\tprints the matching records in the list.\n"                                                       \
    "\tFIELD NAME: [first name, last name, id, phone, debt, date(dd/mm/yyyy)]\n"                         \
    "\tOPERATOR: [<, <=, ==, !=, >=, >]\n"                                                               \
    "\tVALUE: a valid value for the given field\n\n"                                                     \
    "set first name=<value>, last name=<value>, id=<value>, phone=<value>, debt=<value>, date=<value>\n" \
    "\tadd a record to the data structure.\n"                                                            \
    "\tif the id alredy exists it's must match the person's first and last name,\n"                      \
    "\tand the debt and the date will be updated.\n\n"                                                   \
    "print\n"                                                                                            \
    "\tprints a table with all the records.\n\n"                                                         \
    "quit\n\n"

#define PRINT_QUIERIES_HELP fputs(QUIERIES_HELP_TXT, stdout);

enum operators
{
    // less-than, less-equals, equals, not-equals, greater-equals, grater-than
    OP_INVALID_OP = -1,
    OP_LT,
    OP_LE,
    OP_EQ,
    OP_NE,
    OP_GE,
    OP_GT
};

enum person_fields
{
    P_INVALID_FIELD = -1,
    P_FIRST_NAME,
    P_LAST_NAME,
    P_ID,
    P_PHONE,
    P_CURRENT_DEBT,
    P_DATE
};

// operate the overall prompt feature
int prompt(struct Person **head_p_p, FILE *fp);
int route_cmd(char *cmd_str, size_t cmd_len, struct Person **head_p_p, FILE *fp);

int make_select(char *cmd_tok, struct Person *head_p);
int make_set(char *cmd_tok, struct Person **head_p_p, FILE *fp);

// returns wich field is in the str
enum person_fields parse_field(char *field_tok);
enum operators parse_operator(const char *op_str);
enum validation_result validate_cmd_value(const char *value, enum person_fields field);
size_t print_persons_filtered(
    struct Person *head_p, enum operators op, void *value,
    int (*comparison_func_p)(struct Person *person_p, enum operators op, void *value));

// check conditions
int check_first_name_condition(struct Person *person_p, enum operators op, void *value);
int check_last_name_condition(struct Person *person_p, enum operators op, void *value);
int check_id_condition(struct Person *person_p, enum operators op, void *value);
int check_phone_condition(struct Person *person_p, enum operators op, void *value);
int check_amount_condition(struct Person *person_p, enum operators op, void *value);
int check_date_condition(struct Person *person_p, enum operators op, void *given_date);
int check_condition(enum operators op, float cmp_res);
