#include "general_defs.h"


enum validation_result _name_validation(const char *name);
enum validation_result _id_validation(const char *id_str);
enum validation_result _phone_validation(const char *phone);
enum validation_result _amount_validation(const char *amount);
enum validation_result _date_validation(const char *date);
enum validation_result _line_cols_validation(const char *line);
int str_isctype(const char *str, int (*ctype_func_ptr)(int ch), int is_not_char);
size_t str_count_char(const char *str, const char ch);
