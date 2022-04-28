#include "general_defs.h"


enum validation_result _name_validation(const char *name);
enum validation_result _id_validation(const char *id_str);
enum validation_result _phone_validation(const char *phone);
enum validation_result _amount_validation(const char *amount);
enum validation_result _date_validation(const char *date);
enum validation_result _line_cols_validation(const char *line);

/*
 * call _line_cols_validation function with the line
 * and print related error message if the line is invalid.
 * param: line: the string of aline from the file.
 * param: line_n: the line number.
 * return: the validation code of _line_cols_validation.
 * negative value if invalid or 0 when valid.
 */
enum validation_result validate_line_cols(const char *line, size_t line_n);
enum validation_result validate_not_null_column(char *col_str, size_t line_number, size_t col_number);
enum validation_result validate_name(const char *name, const char *first_or_last, size_t line_number);
enum validation_result validate_id(const char *id, size_t line_number);
enum validation_result validate_phone(const char *phone, size_t line_number);
enum validation_result validate_amount(const char *amount, size_t line_number);
enum validation_result validate_date(const char *date, size_t line_number);
enum validation_result validate_date_parsing(struct Date *date);
// validate the value for the given field on the command (query).
enum validation_result validate_cmd_value(const char *value, enum person_fields field);

/*
 * return 1 if all the chars in *str return 1 for the given function pointer.
 * you can turn the check for evry char to !ctype_function by setting is_not_char to 1,
 * set it to 0 for positive check.
 */
int str_isctype(const char *str, int (*ctype_func_ptr)(int ch), int is_not_char);

// return how many times ch found in str. (zero if not found)
size_t str_count_char(const char *str, const char ch);
