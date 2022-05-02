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
    while (*name)
    {
        if (!isalpha(*name) && *name != ' ')
            return CONTAINS_INVALID_CHARS;
        name++;
    }

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
    
    // double dot ..
    if (str_count_char(amount, '.') > 1)
        return INVALID_FORMAT;
    
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

    // if too much or too little slashes /
    if (str_count_char(date, DATE_DELIM_CHAR) != 2)
        return INVALID_FORMAT;
    // make a copy to work with strtok
    strncpy(date_cpy, date, MAX_DATE_LEN + 1); // + 1 for '\0'

    tok = strtok(date_cpy, DATE_DELIM_STR); // day
    if (_date_token_validation(tok, DATE_MIN_DAY, DATE_MAX_DAY, DATE_MAX_DAY_LEN) != VALID)
        return INVALID_FORMAT;
    
    tok = strtok(NULL, DATE_DELIM_STR); // month
    if (_date_token_validation(tok, DATE_MIN_MONTH, DATE_MAX_MONTH, DATE_MAX_MONTH_LEN) != VALID)
        return INVALID_FORMAT;
    
    tok = strtok(NULL, DATE_DELIM_STR); // year
    if (_date_token_validation(tok, DATE_MIN_YEAR, DATE_MAX_YEAR, DATE_MAX_YEAR_LEN) != VALID)
        return INVALID_FORMAT;

    return VALID;
}

enum validation_result _date_token_validation(char *date_tok, int min_val, int max_val, int max_len)
{
    int temp_n;
    if (date_tok == NULL                        // no token
        || !str_isctype(date_tok, &isdigit, 0)  // not all digits
        || (temp_n = atoi(date_tok)) < min_val  // less then min_val
        || temp_n > max_val                     // greater then max_val
        || strlen(date_tok) > max_len)          // too long string
        return INVALID_FORMAT;
    return VALID;
}

enum validation_result _line_cols_validation(const char *line)
{
    if (*line == '\0')
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

enum validation_result validate_line_cols(const char *line, size_t line_number)
{
    enum validation_result validation_res = _line_cols_validation(line);

    switch (validation_res)
    {
    case EMPTY_STRING:
        fprintf(stderr, "error. empty line on the file. line no. %lu\n", line_number);
        break;
    case TOO_SHORT:
        fprintf(stderr, "error. too short line on the file. line no. %lu\n", line_number);
        break;    
    case MISSING_COLUMNS:
        fprintf(stderr, "error. missing columns on the file. line no. %lu\n", line_number);
        break;
    case TOO_MUCH_COLUMNS:
        fprintf(stderr, "error. too much columns on the file. line no. %lu\n", line_number);
        break;
    }

    return validation_res;
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

    switch (validation_res)
    {
    case EMPTY_STRING:
        fprintf(stderr, "error. empty %s name at line no. %lu\n",
                first_or_last, line_number);
        break;
    case CONTAINS_INVALID_CHARS:
        fprintf(stderr, "error. the %s name \"%s\" contains invalid characters. line no. %lu\n",
                first_or_last, name, line_number);
        break;
    }

    return validation_res;
}

enum validation_result validate_id(const char *id, size_t line_number)
{
    enum validation_result validation_res = _id_validation(id);

    switch (validation_res)
    {
    case EMPTY_STRING:
        fprintf(stderr, "error. empty id at line no. %lu\n", line_number);
        break;
    case TOO_SHORT:
        fprintf(stderr, "error. the id \"%s\" is too short (should be 9 digits). line no. %lu\n",
                id, line_number);
        break;
    case TOO_LONG:
        fprintf(stderr, "error. the id \"%s\" is too long (should be 9 digits). line no. %lu\n",
                id, line_number);
        break;
    case CONTAINS_INVALID_CHARS:
        fprintf(stderr, "error. the id \"%s\" contains invalid chars. (should be digits only). line no. %lu\n",
                id, line_number);
        break;

    default:
        break;
    }

    return validation_res;
}

enum validation_result validate_phone(const char *phone, size_t line_number)
{
    enum validation_result validation_res = _phone_validation(phone);

    switch (validation_res)
    {
    case EMPTY_STRING:
        fprintf(stderr, "error. empty phone at line no. %lu\n", line_number);
        break;
    case TOO_SHORT:
        fprintf(stderr, "error. the phone \"%s\" is too short (should be %d digits). line no. %lu\n",
                phone, PHONE_VALID_LEN, line_number);
        break;
    case TOO_LONG:
        fprintf(stderr, "error. the phone \"%s\" is too long (should be %d digits). line no. %lu\n",
                phone, PHONE_VALID_LEN, line_number);
        break;
    case INVALID_FORMAT:
        fprintf(stderr, "error. the phone \"%s\" is in invalid format. (should start with %s). line no. %lu\n",
                phone, PHONE_PREFIX, line_number);
        break;
    case CONTAINS_INVALID_CHARS:
        fprintf(stderr, "error. the phone \"%s\" contains invalid chars. (should be digits only). line no. %lu\n", phone, line_number);
        break;
    }

    return validation_res;
}

enum validation_result validate_amount(const char *amount, size_t line_number)
{
    enum validation_result validation_res = _amount_validation(amount);
    switch (validation_res)
    {
    case EMPTY_STRING:
        fprintf(stderr, "error. empty amount at line no. %lu\n", line_number);
        break;
    case INVALID_FORMAT:
        fprintf(stderr, "error. the amount \"%s\" is in invalid format. line no. %lu\n",
                amount, line_number);
        break;
    case CONTAINS_INVALID_CHARS:
        fprintf(stderr, "error. the amount \"%s\" contains invalid chars. (should be digits only). line no. %lu\n",
                amount, line_number);
        break;
    }

    return validation_res;
}

enum validation_result validate_date(const char *date, size_t line_number)
{
    enum validation_result validation_res = _date_validation(date);
    switch (validation_res)
    {
    case EMPTY_STRING:
        fprintf(stderr, "error. empty date at line no. %lu\n", line_number);
        break;
    case TOO_SHORT:
        fprintf(stderr, "error. too short date at line no. %lu\n", line_number);
        break;
    case TOO_LONG:
        fprintf(stderr, "error. too long date at line no. %lu\n", line_number);
        break;
    case CONTAINS_INVALID_CHARS:
        fprintf(stderr, "error. the date \"%s\" contains invalid characters. line no. %lu\n", date, line_number);
        break;
    case INVALID_FORMAT:
        fprintf(stderr, "error. the date \"%s\" is in invalid format. line no. %lu\n", date, line_number);
        break;
    }
    return validation_res;
}

enum validation_result validate_date_parsing(struct Date *date)
{
    if (date->day == 0)
    {
        fputs("error while parsing the date.\n", stderr);
        return PROCESSING_FAILED;
    }
    return VALID;
}

enum validation_result validate_cmd_value(const char *value, enum person_fields field)
{
    switch (field)
    {
    case P_FIRST_NAME:
    case P_LAST_NAME:
        return _name_validation(value);
    case P_ID:
        return _id_validation(value);
    case P_PHONE:
        return _phone_validation(value);
    case P_CURRENT_DEBT:
        return _amount_validation(value);
    case P_DATE:
        return _date_validation(value);
    }
}

int str_isctype(const char *str, int (*ctype_func_ptr)(int ch), int is_not_char)
{
    while (*str)
        if (!(*ctype_func_ptr)(*str++) ^ is_not_char)
            return 0;
    return 1;
}

size_t str_count_char(const char *str, const char ch)
{
    size_t count = 0;
    while (*str)
        if (*str++ == ch)
            count++;
    return count;
}
