#include "general_defs.h"
#include "general_functions.h"
#include "validation.h"

/* 
 * creates new Person object from a 'line' of the file.
 * validating all the fields.
 * on success: RESULT_SUCCESS
 * on failure: RESULT_ERROR or EXIT_SIGNAL_ERROR
 */
int create_person_from_line(char *line, size_t line_number, struct Person **new_person_p_p);

// return a Person pointer if new_person_p inserted/updated to the list, else (-invalid id etc.) NULL
struct Person *insert_or_update_person(struct Person **head_p_p, struct Person *new_person_p, size_t line_number);
void update_person_debt_date_phone(struct Person *old_person_p, struct Person *new_person_p);

struct Person *insert_person_at_head(struct Person **head_p_p, struct Person *to_insert_p);
void insert_after(struct Person *prev_p, struct Person *to_insert_p);
// search for the Person in the list.
// return a pointer  to it if found, else NULL.
struct Person *get_person_by_id(struct Person *head_p, const char *id);
// checks if the two Persons hase same first and last names. case insensetive.
int is_same_person_names(struct Person *person_1_p, struct Person *person_2_p);
// remove from the list, NO FREE!
// return: if found: 1, if not found: 0
int remove_person_from_the_list(struct Person **head_p_p, struct Person *person_p);

// free one Person. including the first and last name and the Person itself.
void free_person(struct Person *preson_p);
// free a whole list of Persons
void free_person_linked_list(struct Person *head_p);
