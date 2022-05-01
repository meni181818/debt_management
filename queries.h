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

// operate the overall prompt feature
int prompt(struct Person **head_p_p, FILE *fp);
int route_cmd(char *cmd_str, size_t cmd_len, struct Person **head_p_p, FILE *fp);

int make_select(char *cmd_tok, struct Person *head_p);

/*
 * make set command.
 * parse the command and insert the new records to the data structure,
 * and write it to the file.
 * return: RESULT_SUCCESS or RESULT_ERROR or EXIT_SIGNAL_ERROR
 */
int make_set(char *cmd_tok, struct Person **head_p_p, FILE *fp);

// returns wich field is in the str
enum person_fields parse_field(char *field_tok);
enum operators parse_operator(const char *op_str);
/*
 * loop on the persons linked list and print the matching objects.
 * params: head pointer, operator, value (-to check the condition).
 *         comparison_func_p => pointer to a comparison function.
 * return: the number of matching persons.
 */
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
