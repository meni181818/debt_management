#include "validation.h"

/*
 * return:
 * 0: valid
 * -1: no id (empty string)
 * -2: too short
 * -3: too long
 * -4: contains non-numeric chars
 */
enum validation_result _id_validation(const char *id_str)
{
    // no id
    if (*id_str == '\0')
        return EMPTY_STRING;

    size_t id_len = strlen(id_str);

    // too short
    if (id_len < ID_VALID_LEN)
        return TOO_SHORT;
    // too long
    if (id_len > ID_VALID_LEN)
        return TOO_LONG;

    // contains non-digit chars
    if (!str_isctype(id_str, &isdigit, 0))
        return CONTAINS_INVALID_CHARS;

    return VALID;
}

/*
 * return:
 * 0: valid
 * -1: empty name
 * -4: contains non alphabetic chars
 */
enum validation_result _name_validation(const char *name)
{
    // no name
    if (*name == '\0')
        return EMPTY_STRING;
    if (!str_isctype(name, &isalpha, 0))
        return CONTAINS_INVALID_CHARS;

    return VALID;
}

/*
 * return:
 * 0: valid
 * -1: no phone (empty string)
 * -2: too short
 * -3: too long
 * -5: does not start with zero
 * -4: contains non-digit chars
 */
enum validation_result _phone_validation(const char *phone)
{
    // no phone
    if (*phone == '\0')
        return EMPTY_STRING;

    // check phone length
    size_t phone_len = strlen(phone);
    if (phone_len < PHONE_VALID_LEN)
        return TOO_SHORT;
    if (phone_len > PHONE_VALID_LEN)
        return TOO_LONG;

    // invalid prefix
    if (strncmp(phone, PHONE_PREFIX, strlen(PHONE_PREFIX)) != 0)
        return INVALID_FORMAT;

    // not all digits
    if (!str_isctype(phone, &isdigit, 0))
        return CONTAINS_INVALID_CHARS;

    return VALID;
}

enum validation_result _amount_validation(const char *amount)
{
    // no amount
    if (*amount == '\0' || *amount == '-' && amount[1] == '\0')
        return EMPTY_STRING;

    // not all digits
    // dont check the minus sign with isdigit()
    if (*amount == '-')
        amount++;
    while (*amount) // validate there only digits and .
    {
        if (!isdigit(*amount) && *amount != '.')
            return CONTAINS_INVALID_CHARS;
        amount++;
    }

    return VALID;
}

enum validation_result _date_validation(const char *date)
{
    size_t len, i;
    char date_cpy[MAX_DATE_LEN + 1];
    char *tok;
    int temp_n;
    // no date
    if (*date == '\0')
        return EMPTY_STRING;

    // check the length
    len = strlen(date);
    if (len < MIN_DATE_LEN)
        return TOO_SHORT;
    if (len > MAX_DATE_LEN)
        return TOO_LONG;

    // check for invalid chars (other than digits or /)
    for (i = 0; i < len; i++)
        if (!isdigit(date[i]) && date[i] != '/')
            return CONTAINS_INVALID_CHARS;

    // if too much slashes /
    if (str_count_char(date, DATE_DELIM_CHAR) != 2)
        return INVALID_FORMAT;
    // make a copy to work with strtok
    strncpy(date_cpy, date, MAX_DATE_LEN + 1); // + 1 for '\0'

    // day
    tok = strtok(date_cpy, DATE_DELIM_STR);
    if (tok == NULL || !str_isctype(tok, &isdigit, 0) || (temp_n = atoi(tok)) < DATE_MIN_DAY || temp_n > DATE_MAX_DAY || strlen(tok) > 2)
        return INVALID_FORMAT;

    // month
    tok = strtok(NULL, DATE_DELIM_STR);
    if (tok == NULL || !str_isctype(tok, &isdigit, 0) || (temp_n = atoi(tok)) < DATE_MIN_MONTH || temp_n > DATE_MAX_MONTH || strlen(tok) > 2)
        return INVALID_FORMAT;

    // year
    tok = strtok(NULL, DATE_DELIM_STR);
    if (tok == NULL || !str_isctype(tok, &isdigit, 0) || (temp_n = atoi(tok)) < DATE_MIN_YEAR || temp_n > DATE_MAX_YEAR || strlen(tok) > 4)
        return INVALID_FORMAT;

    return VALID;
}

enum validation_result _line_cols_validation(const char *line)
{
    if (*line == '\0' || *line == '\n' && *(line + 1) == '\0')
        return EMPTY_STRING;

    size_t line_len = strlen(line);
    if (line_len < MIN_LINE_LEN)
        return TOO_SHORT;

    size_t cols_n = str_count_char(line, FILE_DELIM_CHAR) + 1;
    char two_delims[3] = {FILE_DELIM_CHAR, FILE_DELIM_CHAR, '\0'};
    // missing ',' or two delims together
    if (cols_n < FILE_COLUMNS || strstr(line, two_delims) != NULL)
        return MISSING_COLUMNS;
    if (cols_n > FILE_COLUMNS)
        return TOO_MUCH_COLUMNS;
    // delim at the start or at the end of the line
    if (*line == FILE_DELIM_CHAR || line[line_len - 1] == FILE_DELIM_CHAR)
        return MISSING_COLUMNS;

    return VALID;
}

/*
 * return 1 if all the chars in *str return 1 for the given function pointer.
 * you can turn the check for evry char to !ctype_function by setting is_not_char to 1,
 * set it to 0 for positive check.
 */
int str_isctype(const char *str, int (*ctype_func_ptr)(int ch), int is_not_char)
{
    while (*str)
        if (!(*ctype_func_ptr)(*str++) ^ is_not_char)
            return 0;
    return 1;
}

// return how many times ch found in *str. (zero if not found)
size_t str_count_char(const char *str, const char ch)
{
    size_t count = 0;
    while (*str)
        if (*str++ == ch)
            count++;
    return count;
}
