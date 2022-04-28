#include "general_defs.h"

/*
 * creates Date object out of date_str.
 * return: Date object.
 * in case of error while parsing the date_str =>
 *    zeroed Date will be returned
 */
struct Date str_to_date(char *date_str);
// prints the entire list of people in tabular form
void print_persons_records(struct Person *head_p);
/*
 * compare Date objects.
 * return like strcmp.
 * 0 if equals,
 * positive value if dt_1_p > dt_2_p,
 * negative value if dt_1_p < dt_2_p
 */
int date_compare(struct Date *dt_1_p, struct Date *dt_2_p);
/*
 * let the user select (prints the description) an option from 1 to options_n.
 * return: the chosen number.
 */
int get_user_selection_1_9(const char *description, int options_n);
// clear chars from the input file stream until '\n' or EOF is reached
void finish_line(FILE *fp);
/*
 * replace the old sub-str in str_in with new sub-str.
 * both must have the same length.
 */
char *str_replace_in_place(char *str_in, char *old, char *new);
/*
 * try to malloc() and in case of failure let the user select
 * try again or exit.
 * return: not-NULL if success, NULL if failed and user want to exit.
 */
void *try_malloc(size_t size);
/*
 * 'strip' leading and trailing whitespace.
 * if leading spaces => move the pointer of *str to skip it.
 * if trailing spaces => replace it with '\0'
 * return: pointer to the new beginning
 */
char *str_strip_in_place(char *str);
