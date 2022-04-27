#include "data_structure_build.h"

// on success: pointer to a new Person object.
// on failure: NULL
int create_person_from_line(char *line, size_t line_number, struct Person **new_person_p_p) // TODO: seperate to several functions <<
{
    size_t cols_count = 1;
    int ret_val = RESULT_ERROR;
    *new_person_p_p = (struct Person *)try_malloc(sizeof(struct Person));
    if (*new_person_p_p == NULL) // malloc faild and the user want to exit
        return EXIT_SIGNAL;
    
    char *tok = strtok(line, FILE_DELIM_STR);

    do
    {
        // if invalid: break out and free, return NULL.
        // if valid: insert to new_person_p
        // first name
        if (validate_not_null_column(tok, line_number, cols_count) != VALID || validate_name(tok, "first", line_number) != VALID)
            break;
        (*new_person_p_p)->first_name = (char *)try_malloc((strlen(tok) + 1) * sizeof(char));
        if ((*new_person_p_p)->first_name == NULL) // malloc faild and the user want to exit
        {
            ret_val = EXIT_SIGNAL;
            break;
        }
        strcpy((*new_person_p_p)->first_name, tok);
        cols_count++;

        // last name
        tok = strtok(NULL, FILE_DELIM_STR);
        if (validate_not_null_column(tok, line_number, cols_count) != VALID || validate_name(tok, "last", line_number) != VALID)
            break;
        (*new_person_p_p)->last_name = (char *)try_malloc((strlen(tok) + 1) * sizeof(char));
        if ((*new_person_p_p)->last_name == NULL) // malloc faild and the user want to exit
        {
            ret_val = EXIT_SIGNAL;
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
        (*new_person_p_p)->first_trans_date = str_to_date(tok);
        // validate we got 3 tokens
        if (validate_date_parsing(&(*new_person_p_p)->first_trans_date) != VALID)
            break;

        (*new_person_p_p)->next_p = NULL;

        return RESULT_SUCCESS;
    } while (0);

    // in case of failure, we breaking out of the do-while
    free_person((*new_person_p_p));
    return ret_val;
}

enum validation_result validate_not_null_column(char *col_str, size_t line_number, size_t col_number)
{
    if (col_str == NULL)
    {
        fprintf(stderr, "error. NULL column on line no. %lu column no. %lu.\n", line_number, col_number);
        return NULL_INPUT;
    }
    return VALID;
}

enum validation_result validate_name(const char *name, const char *first_or_last, size_t line_number)
{
    enum validation_result validation_res = _name_validation(name);
    if (validation_res == EMPTY_STRING)
        fprintf(stderr, "error. empty %s name at line no. %lu\n", first_or_last, line_number);
    else if (validation_res == CONTAINS_INVALID_CHARS)
        fprintf(stderr, "error. the %s name \"%s\" contains invalid characters. line no. %lu\n", first_or_last, name, line_number);

    return validation_res;
}

enum validation_result validate_id(const char *id, size_t line_number)
{
    enum validation_result validation_res = _id_validation(id);
    if (validation_res == EMPTY_STRING)
        fprintf(stderr, "error. empty id at line no. %lu\n", line_number);
    else if (validation_res == TOO_SHORT)
        fprintf(stderr, "error. the id \"%s\" is too short (should be 9 digits). line no. %lu\n", id, line_number);
    else if (validation_res == TOO_LONG)
        fprintf(stderr, "error. the id \"%s\" is too long (should be 9 digits). line no. %lu\n", id, line_number);
    else if (validation_res == CONTAINS_INVALID_CHARS)
        fprintf(stderr, "error. the id \"%s\" contains invalid chars. (should be digits only). line no. %lu\n", id, line_number);

    return validation_res;
}

enum validation_result validate_phone(const char *phone, size_t line_number)
{
    enum validation_result validation_res = _phone_validation(phone);
    if (validation_res == EMPTY_STRING)
        fprintf(stderr, "error. empty phone at line no. %lu\n", line_number);
    else if (validation_res == TOO_SHORT)
        fprintf(stderr, "error. the phone \"%s\" is too short (should be %d digits). line no. %lu\n", phone, PHONE_VALID_LEN, line_number);
    else if (validation_res == TOO_LONG)
        fprintf(stderr, "error. the phone \"%s\" is too long (should be %d digits). line no. %lu\n", phone, PHONE_VALID_LEN, line_number);
    else if (validation_res == INVALID_FORMAT)
        fprintf(stderr, "error. the phone \"%s\" is in invalid format. (should start with %s). line no. %lu\n", phone, PHONE_PREFIX, line_number);
    else if (validation_res == CONTAINS_INVALID_CHARS)
        fprintf(stderr, "error. the phone \"%s\" contains invalid chars. (should be digits only). line no. %lu\n", phone, line_number);

    return validation_res;
}

enum validation_result validate_amount(const char *amount, size_t line_number)
{
    enum validation_result validation_res = _amount_validation(amount);
    if (validation_res == EMPTY_STRING)
        fprintf(stderr, "error. empty amount at line no. %lu\n", line_number);
    else if (validation_res == CONTAINS_INVALID_CHARS)
        fprintf(stderr, "error. the amount \"%s\" contains invalid chars. (should be digits only). line no. %lu\n", amount, line_number);

    return validation_res;
}

enum validation_result validate_date(const char *date, size_t line_number)
{
    enum validation_result validation_res = _date_validation(date);
    if (validation_res == EMPTY_STRING)
        fprintf(stderr, "error. empty date at line no. %lu\n", line_number);
    else if (validation_res == TOO_SHORT)
        fprintf(stderr, "error. too short date at line no. %lu\n", line_number);
    else if (validation_res == TOO_LONG)
        fprintf(stderr, "error. too long date at line no. %lu\n", line_number);
    else if (validation_res == CONTAINS_INVALID_CHARS)
        fprintf(stderr, "error. the date \"%s\" contains invalid characters. line no. %lu\n", date, line_number);
    else if (validation_res == INVALID_FORMAT)
        fprintf(stderr, "error. the date \"%s\" is in invalid format. line no. %lu\n", date, line_number);

    return validation_res;
}

enum validation_result validate_date_parsing(struct Date *date)
{
    if (date->day == 0)
    {
        fputs("error while parsing the date.\n", stderr);
        return PROCESSING_FAILD;
    }
    return VALID;
}

// return NULL if no valid person loaded from the line, or a pointer to if loaded 1 valid person
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
            update_person_debt_and_date(old_person_p, new_person_p);
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

/*
 * call _line_cols_validation function with the line
 * and print related error message if the line invalid.
 * param: line: the string of aline from the file.
 * param: line_n: the line number.
 * return: the validation code of _line_cols_validation.
 * negative value if invalid or 0 when valid.
 */
enum validation_result validate_line_cols(const char *line, size_t line_number)
{
    enum validation_result validation_res = _line_cols_validation(line);
    if (validation_res == EMPTY_STRING)
        fprintf(stderr, "error. empty line on the file. line no. %lu\n", line_number);
    else if (validation_res == TOO_SHORT)
        fprintf(stderr, "error. too short line on the file. line no. %lu\n", line_number);
    else if (validation_res == MISSING_COLUMNS)
        fprintf(stderr, "error. missing columns on the file. line no. %lu\n", line_number);
    else if (validation_res == TOO_MUCH_COLUMNS)
        fprintf(stderr, "error. too much columns on the file. line no. %lu\n", line_number);

    return validation_res;
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

void update_person_debt_and_date(struct Person *old_person_p, struct Person *new_person_p)
{
    old_person_p->current_debt += new_person_p->current_debt;
    // if the new_person have older date => update the date.
    if (date_compare(&old_person_p->first_trans_date, &new_person_p->first_trans_date) > 0)
        old_person_p->first_trans_date = new_person_p->first_trans_date;
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

    while (prev_p->next_p != person_p && prev_p->next_p != NULL)
    {
        prev_p = prev_p->next_p;
    }

    // if got to the end
    if (prev_p->next_p == NULL)
        return 0;
    // else: got befor the wanted element
    prev_p->next_p = prev_p->next_p->next_p;
    return 1;
}
