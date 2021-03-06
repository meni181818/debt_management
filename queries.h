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
    "\tFIELD NAME: [first name|last name|id|phone|debt|date(dd/mm/yyyy)]\n"                              \
    "\tOPERATOR: [<|<=|=|!=|>=|>]\n"                                                                     \
    "\tVALUE: a valid value for the given field\n\n"                                                     \
    "set first name=<value>, last name=<value>, id=<value>, phone=<value>, debt=<value>, date=<value>\n" \
    "\tadd a record to the data structure.\n"                                                            \
    "\tif the id alredy exists it's must match the person's first and last name,\n"                      \
    "\tand the debt and the date will be updated.\n\n"                                                   \
    "print\n"                                                                                            \
    "\tprints a table with all the records.\n\n"                                                         \
    "quit\n\n"

// comparison function-pointer type
typedef int (*comparison_func_p_t) (struct Person *, enum operators, void *);

// operate the overall prompt feature
int prompt(struct Person **head_p_p, FILE *fp);
// route the command input to the right function to handle it
int route_cmd(char *cmd_str, size_t cmd_len, struct Person **head_p_p, FILE *fp);

/*
 * make select command.
 * parse the command, the field name, the operator, the value,
 * validate those, and calls print_persons_filtered() with the right
 * function pointer for filtering.
 * return RESULT_SUCCESS or RESULT_ERROR
 */
int make_select(char *cmd_tok, struct Person *head_p);

/*
 * make set command.
 * parse the command and insert the new records to the data structure,
 * and write it to the file.
 * return: RESULT_SUCCESS or RESULT_ERROR or EXIT_SIGNAL_ERROR
 */
int make_set(char *cmd_tok, struct Person **head_p_p, FILE *fp);

// returns wich field is in the string
enum person_fields parse_field(char *field_tok);
// returns wich operator is in the string
enum operators parse_operator(const char *op_str, size_t op_len);
/*
 * loop on the persons linked list and print the matching objects.
 * params: head pointer, operator, value (-to check the condition).
 *      comparison_func_p => pointer to a comparison function.
 * return: the number of matching persons.
 */
size_t print_persons_filtered(
    struct Person *head_p, enum operators op, void *value,
    int (*comparison_func_p)(struct Person *person_p, enum operators op, void *value));

// check conditions for evry field in Person struct
int check_first_name_condition(struct Person *person_p, enum operators op, void *value);
int check_last_name_condition(struct Person *person_p, enum operators op, void *value);
int check_id_condition(struct Person *person_p, enum operators op, void *value);
int check_phone_condition(struct Person *person_p, enum operators op, void *value);
int check_amount_condition(struct Person *person_p, enum operators op, void *value);
int check_date_condition(struct Person *person_p, enum operators op, void *given_date);
// condition checking
int check_condition(enum operators op, float cmp_res);
