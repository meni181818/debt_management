#include "general_defs.h"

struct Date str_to_date(char *date_str);
void print_persons_records(struct Person *head_p);
int date_compare(struct Date *dt_1_p, struct Date *dt_2_p);
int get_user_selection_1_9(const char *description, int options_n);
void finish_line(FILE *fp);
char *str_replace_in_place(char *str_in, char *old, char *new);
