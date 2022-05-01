#include "data_structure_build.h"

int create_person_from_line(char *line, size_t line_number, struct Person **new_person_p_p)
{
    size_t cols_count = 1;
    int ret_val = RESULT_ERROR;
    *new_person_p_p = (struct Person *)try_malloc(sizeof(struct Person));
    if (*new_person_p_p == NULL) // malloc failed and the user want to exit
        return EXIT_SIGNAL_ERROR;

    char *tok = strtok(line, FILE_DELIM_STR);

    do
    {
        // if invalid: break out and free, return NULL.
        // if valid: insert to new_person_p
        // first name
        if (validate_not_null_column(tok, line_number, cols_count) != VALID || validate_name(tok, "first", line_number) != VALID)
            break;
        (*new_person_p_p)->first_name = (char *)try_malloc((strlen(tok) + 1) * sizeof(char));
        if ((*new_person_p_p)->first_name == NULL) // malloc failed and the user want to exit
        {
            ret_val = EXIT_SIGNAL_ERROR;
            break;
        }
        strcpy((*new_person_p_p)->first_name, tok);
        cols_count++;

        // last name
        tok = strtok(NULL, FILE_DELIM_STR);
        if (validate_not_null_column(tok, line_number, cols_count) != VALID || validate_name(tok, "last", line_number) != VALID)
            break;
        (*new_person_p_p)->last_name = (char *)try_malloc((strlen(tok) + 1) * sizeof(char));
        if ((*new_person_p_p)->last_name == NULL) // malloc failed and the user want to exit
        {
            ret_val = EXIT_SIGNAL_ERROR;
            break;
        }
        strcpy((*new_person_p_p)->last_name, tok);
        cols_count++;

        // id
        tok = strtok(NULL, FILE_DELIM_STR);
        if (validate_not_null_column(tok, line_number, cols_count) != VALID || validate_id(tok, line_number) != VALID)
            break;
        strncpy((*new_person_p_p)->id, tok, ID_VALID_LEN + 1); // + 1 for '\0'
        cols_count++;

        // phone
        tok = strtok(NULL, FILE_DELIM_STR);
        if (validate_not_null_column(tok, line_number, cols_count) != VALID || validate_phone(tok, line_number) != VALID)
            break;
        strncpy((*new_person_p_p)->phone, tok, PHONE_VALID_LEN + 1); // + 1 for '\0'
        cols_count++;

        // amount
        tok = strtok(NULL, FILE_DELIM_STR);
        if (validate_not_null_column(tok, line_number, cols_count) != VALID || validate_amount(tok, line_number) != VALID)
            break;
        (*new_person_p_p)->current_debt = atof(tok);
        cols_count++;

        // date
        tok = strtok(NULL, FILE_DELIM_STR);
        if (validate_not_null_column(tok, line_number, cols_count) != VALID || validate_date(tok, line_number) != VALID)
            break;
        (*new_person_p_p)->erliest_date = (*new_person_p_p)->latest_date = str_to_date(tok);
        // validate we got 3 tokens
        if (validate_date_parsing(&(*new_person_p_p)->erliest_date) != VALID)
            break;

        (*new_person_p_p)->next_p = NULL;

        return RESULT_SUCCESS;
    } while (0);

    // in case of failure, we breaking out of the do-while
    free_person((*new_person_p_p));
    return ret_val;
}

struct Person *insert_or_update_person(struct Person **head_p_p, struct Person *new_person_p, size_t line_number)
{
    struct Person *old_person_p;
    struct Person *ret = NULL;
    if (new_person_p == NULL)
        return ret;

    // search if the person alredy exists
    old_person_p = get_person_by_id(*head_p_p, new_person_p->id);
    if (old_person_p != NULL)
    {
        // same id, but same names?
        if (is_same_person_names(old_person_p, new_person_p))
        {
            update_person_debt_date_phone(old_person_p, new_person_p);
            ret = old_person_p;
        }
        else // conflicting names for the same id
        {
            fprintf(stderr, "conflicting names for the same id. id:\"%s\", \"%s %s\" != \"%s %s\". line no. %lu\n",
                    old_person_p->id, old_person_p->first_name, old_person_p->last_name,
                    new_person_p->first_name, new_person_p->last_name, line_number);
        }
        // anyway free the new
        free_person(new_person_p);
    }
    else // no old person with the same id
    {
        insert_person_at_head(head_p_p, new_person_p);
        ret = new_person_p;
    }
    return ret;
}

void free_person(struct Person *preson_p)
{
    free(preson_p->first_name);
    free(preson_p->last_name);
    free(preson_p);
}

void free_person_linked_list(struct Person *head_p)
{
    struct Person *tmp_next_p;
    while (head_p != NULL)
    {
        tmp_next_p = head_p->next_p;
        free_person(head_p);
        head_p = tmp_next_p;
    }
}

void insert_after(struct Person *prev_p, struct Person *to_insert_p)
{
    to_insert_p->next_p = prev_p->next_p;
    prev_p->next_p = to_insert_p;
}

// pointer to a pointer to change the value of head in the caller stack
struct Person *insert_person_at_head(struct Person **head_p_p, struct Person *to_insert_p)
{
    to_insert_p->next_p = *head_p_p;
    *head_p_p = to_insert_p;
    return *head_p_p;
}

struct Person *get_person_by_id(struct Person *head_p, const char *id)
{
    while (head_p != NULL)
    {
        if (strncmp(head_p->id, id, ID_VALID_LEN) == 0)
            return head_p;
        head_p = head_p->next_p;
    }
    return NULL;
}

int is_same_person_names(struct Person *person_1_p, struct Person *person_2_p)
{
    return (strcmp(person_1_p->first_name, person_2_p->first_name) == 0 && strcmp(person_1_p->last_name, person_2_p->last_name) == 0);
}

void update_person_debt_date_phone(struct Person *old_person_p, struct Person *new_person_p)
{
    old_person_p->current_debt += new_person_p->current_debt;
    // if the new_person have older date => update the date.
    if (date_compare(&old_person_p->erliest_date, &new_person_p->erliest_date) > 0)
        old_person_p->erliest_date = new_person_p->erliest_date;
    // update the phone if it's newer
    if (date_compare(&old_person_p->latest_date, &new_person_p->latest_date) <= 0)
        strncpy(old_person_p->phone, new_person_p->phone, PHONE_VALID_LEN);
}

// remove from the list, NO FREE!
// return: if found: 1, if not found: 0
int remove_person_from_the_list(struct Person **head_p_p, struct Person *person_p)
{
    struct Person *prev_p = *head_p_p;
    // no list
    if (*head_p_p == NULL)
        return 0;
    // we want to remove the first element
    if (*head_p_p == person_p)
    {
        *head_p_p = (*head_p_p)->next_p;
        return 1;
    }

    // loop untile we found the person to remove or hit the end
    while (prev_p->next_p != person_p && prev_p->next_p != NULL)
        prev_p = prev_p->next_p;
    // check why we out of the loop:
    // if got to the end
    if (prev_p->next_p == NULL)
        return 0;
    // else: got befor the wanted element
    prev_p->next_p = prev_p->next_p->next_p;
    return 1;
}
