#include "general_defs.h"
#include "general_functions.h"
#include "validation.h"

struct Person *create_person_from_line(char *line, size_t line_number);

enum validation_result validate_line_cols(const char *line, size_t line_n);
enum validation_result validate_not_null_column(char *col_str, size_t line_number, size_t col_number);
enum validation_result validate_name(const char *name, const char *first_or_last, size_t line_number);
enum validation_result validate_id(const char *id, size_t line_number);
enum validation_result validate_phone(const char *phone, size_t line_number);
enum validation_result validate_amount(const char *amount, size_t line_number);
enum validation_result validate_date(const char *date, size_t line_number);
enum validation_result validate_date_parsing(struct Date *date);


struct Person *insert_or_update_person(struct Person **head_p_p, struct Person *new_person_p, size_t line_number);
void update_person_debt_and_date(struct Person *old_person_p, struct Person *new_person_p);
struct Person *insert_person_at_head(struct Person **head_p_p, struct Person *to_insert_p);
struct Person *get_person_by_id(struct Person *head_p, const char *id);
int is_same_person_names(struct Person *person_1_p, struct Person *person_2_p);
int remove_person_from_the_list(struct Person **head_p_p, struct Person *person_p);

void free_person(struct Person *preson_p);
void free_person_linked_list(struct Person *head_p);

void print_persons_records(struct Person *head_p);