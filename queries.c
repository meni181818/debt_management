#include "queries.h"

int prompt(struct Person **head_p_p, FILE *fp)
{
    char cmd_buf[INPUT_LINE_BUF_SIZE];
    size_t cmd_len;

    fputs("you can now enter commands. for help type \"help\"\n", stdout);
    while (1)
    {
        fputs(">>> ", stdout);
        fgets(cmd_buf, INPUT_LINE_BUF_SIZE, stdin);
        
        cmd_len = strnlen(cmd_buf, INPUT_LINE_BUF_SIZE);
        if (cmd_buf[cmd_len - 1] != '\n') // no '\n' at the end => the line is too long
        {
            fputs("the command is too long. (up to 254 characters). please try again\n", stderr);
            // clear the rest of the chars from stdin
            finish_line(stdin);
            continue;
        }
        // trim the right '\n'
        RTRIM_NEW_LINE(cmd_buf, cmd_len);

        if (*cmd_buf == '\0') // just hit enter - no command
            continue;

        // replace "first name" with "first_name". same with last name.
        str_replace_in_place(cmd_buf, "first name", "first_name");
        str_replace_in_place(cmd_buf, "last name", "last_name");

        // 'quit'
        if (route_cmd(cmd_buf, cmd_len, head_p_p, fp) == EXIT_SIGNAL)
            break;
    }
    return RESULT_SUCCESS;
}

int route_cmd(char *cmd_str, size_t cmd_len, struct Person **head_p_p, FILE *fp)
{
    char *tok = strtok(cmd_str, " ");
    if (tok == NULL) // just hit space\s and enter
        return RESULT_ERROR;

    if (strcmp(tok, "select") == 0)
        return make_select(tok, *head_p_p);

    if (strcmp(tok, "set") == 0)
        return make_set(tok, head_p_p, fp);

    if (strcmp(tok, "print") == 0)
    {
        print_persons_records(*head_p_p);
        return RESULT_SUCCESS;
    }

    if (strcmp(tok, "quit") == 0)
        return EXIT_SIGNAL;

    if (strcmp(tok, "help") == 0)
    {
        PRINT_QUIERIES_HELP;
        return RESULT_SUCCESS;
    }

    // non of the above => unknown command
    PRINT_INVALID_CMD("unknown command");

    return RESULT_ERROR;
}
// set first name=eti, last name=grossman, id=206165532, phone=0504137649, debt=10, date=26/4/2022
int make_set(char *cmd_tok, struct Person **head_p_p, FILE *fp)
{
    char *fields_p_arr[PERSON_FIELDS_N] = {0}; // zero the arr
    enum person_fields current_field;
    char *value_str_p;
    size_t overall_len = FILE_COLUMNS - 1 + 1; // (5 * ',' + '\0')
    char final_line_for_validation[INPUT_LINE_BUF_SIZE], final_line_for_file[INPUT_LINE_BUF_SIZE];
    struct Person *new_person;

    // we now at the first token ("set"). loop 6 times for all person fields.
    for (size_t i = 0; i < PERSON_FIELDS_N; i++)
    {
        cmd_tok = strtok(NULL, " ,"); // potential problem <<<<<<<<<<<<<<<<<<<<<<<<
        if (cmd_tok == NULL)          // missing token
        {
            PRINT_INVALID_CMD("missing field");
            return RESULT_ERROR;
        }

        value_str_p = strchr(cmd_tok, '=');
        // if no value placing (<field_name>=<value>)
        if (value_str_p == NULL)
        {
            PRINT_INVALID_CMD("missing value(=). usage: <field_name>=<value>");
            return RESULT_ERROR;
        }
        // temporarily replace '=' with '\0' for parsing the field name
        *value_str_p = '\0';
        current_field = parse_field(cmd_tok);
        *value_str_p = '='; // bring it back

        // check if we got duplicate field names (we zero that array above)
        if (fields_p_arr[current_field] != 0)
        {
            PRINT_INVALID_CMD("duplicate fields");
            return RESULT_ERROR;
        }
        // point it to it's next char (=> the actual value string) and save it on the fields array
        fields_p_arr[current_field] = ++value_str_p;
        overall_len += strlen(value_str_p);
    }

    if (overall_len > INPUT_LINE_BUF_SIZE - 1)
    {
        PRINT_INVALID_CMD("the line is too long");
        return RESULT_ERROR;
    }

    // make line like in the file (for person object creation)
    snprintf(final_line_for_validation, INPUT_LINE_BUF_SIZE, "%s,%s,%s,%s,%s,%s",
             fields_p_arr[0], fields_p_arr[1], fields_p_arr[2],
             fields_p_arr[3], fields_p_arr[4], fields_p_arr[5]);
    // copy it before validation (validation using strtok and mutate it)
    strncpy(final_line_for_file, final_line_for_validation, INPUT_LINE_BUF_SIZE);

    new_person = create_person_from_line(final_line_for_validation, 0);
    // invalid field\s
    if (new_person == NULL)
        return RESULT_ERROR;
    new_person = insert_or_update_person(head_p_p, new_person, 0);
    remove_person_from_the_list(head_p_p, new_person);
    insert_person_in_order(head_p_p, new_person);

    // write the new record line to the file
    fprintf(fp, "%s\n", final_line_for_file);
    return RESULT_SUCCESS;
}

int make_select(char *cmd_tok, struct Person *head_p)
{
    enum person_fields field_type;
    enum operators op;
    // pointer to the value to compare
    void *value_p;
    // arrayy of pointers to comparison functions
    int (*comparison_funcs_arr[PERSON_FIELDS_N])(struct Person * person_p, enum operators op, void *value) = {
        &check_first_name_condition, &check_last_name_condition,
        &check_id_condition, &check_phone_condition,
        &check_amount_condition, &check_date_condition};
    size_t found_count = 0;

    // we now at the first token ("select"). move to the next tok to point at the field name.
    cmd_tok = strtok(NULL, " ");
    if (cmd_tok == NULL)
    {
        PRINT_INVALID_CMD("missing field");
        return RESULT_ERROR;
    }

    // get the field
    field_type = parse_field(cmd_tok);
    if (field_type == P_INVALID_FIELD)
    {
        PRINT_INVALID_CMD("invalid field");
        return RESULT_ERROR;
    }

    // we now at the field token. move to the next tok to point at the operator.
    cmd_tok = strtok(NULL, " ");
    if (cmd_tok == NULL)
    {
        PRINT_INVALID_CMD("missing operator");
        return RESULT_ERROR;
    }

    // get the operator
    op = parse_operator(cmd_tok);
    if (op == OP_INVALID_OP)
    {
        PRINT_INVALID_CMD("invalid operator");
        return RESULT_ERROR;
    }

    // move to the value to compare to
    cmd_tok = strtok(NULL, " ");
    if (cmd_tok == NULL)
    {
        PRINT_INVALID_CMD("missing value to compare");
        return RESULT_ERROR;
    }

    // validate the value
    if (validate_cmd_value(cmd_tok, field_type) != VALID)
    {
        PRINT_INVALID_CMD("invalid value");
        return RESULT_ERROR;
    }

    // check there is no more args..
    if (strtok(NULL, " ") != NULL)
    {
        PRINT_INVALID_CMD("too many args");
        return RESULT_ERROR;
    }

    // if the field is: first/last name/id/phone: use the cmd_tok as value to compare (string)
    // if the field is: debt/date: convert it to float/Date to use in the comparison function
    if (field_type == P_CURRENT_DEBT)
    {
        float given_debt = atof(cmd_tok);
        value_p = &given_debt;
    }
    else if (field_type == P_DATE)
    {
        struct Date given_date = str_to_date(cmd_tok);
        value_p = &given_date;
    }
    else
    {
        value_p = cmd_tok;
    }

    // print matching result
    found_count = print_persons_filtered(head_p, op, value_p, comparison_funcs_arr[field_type]);
    // if no result
    if (!found_count)
        fputs("no results found for the query\n", stdout);
    return RESULT_SUCCESS;
}

enum person_fields parse_field(char *field_tok)
{
    if (strcmp(field_tok, "first_name") == 0)
        return P_FIRST_NAME;
    if (strcmp(field_tok, "last_name") == 0)
        return P_LAST_NAME;
    if (strcmp(field_tok, "id") == 0)
        return P_ID;
    if (strcmp(field_tok, "phone") == 0)
        return P_PHONE;
    if (strcmp(field_tok, "debt") == 0)
        return P_CURRENT_DEBT;
    if (strcmp(field_tok, "date") == 0)
        return P_DATE;
    return P_INVALID_FIELD;
}

enum operators parse_operator(const char *op_str)
{
    if (strcmp(op_str, "<") == 0)
        return OP_LT;
    if (strcmp(op_str, "<=") == 0)
        return OP_LE;
    if (strcmp(op_str, "==") == 0)
        return OP_EQ;
    if (strcmp(op_str, "!=") == 0)
        return OP_NE;
    if (strcmp(op_str, ">=") == 0)
        return OP_GE;
    if (strcmp(op_str, ">") == 0)
        return OP_GT;
    return OP_INVALID_OP;
}

// validate the value for the given field on the command. negative values when invalid. 0 when valid.
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

/*
 * loop on the persons linked list and print the matching objects.
 * params: head pointer, operator, value (-to check the condition).
 *         comparison_func_p => pointer to a comparison function.
 * return: the number of matching persons.
 */
size_t print_persons_filtered(
    struct Person *head_p, enum operators op, void *value,
    int (*comparison_func_p)(struct Person *person_p, enum operators op, void *value))
{
    size_t count_prints = 0;
    while (head_p != NULL)
    {
        if ((*comparison_func_p)(head_p, op, value))
        {
            // if it'sthe first found => print header first
            if (count_prints == 0)
                fputs(PERSONS_TABLE_HEADER, stdout);
            PRINT_PERSON(head_p);
            count_prints++;
        }
        head_p = head_p->next_p;
    }
    // new line after the table
    if (count_prints)
        fputc('\n', stdout);
    return count_prints;
}

int check_first_name_condition(struct Person *person_p, enum operators op, void *value)
{
    return check_condition(op, (float)strcmp(person_p->first_name, (char *)value));
}

int check_last_name_condition(struct Person *person_p, enum operators op, void *value)
{
    return check_condition(op, (float)strcmp(person_p->last_name, (char *)value));
}

int check_id_condition(struct Person *person_p, enum operators op, void *value)
{
    return check_condition(op, (float)strcmp(person_p->id, (char *)value));
}

int check_phone_condition(struct Person *person_p, enum operators op, void *value)
{
    return check_condition(op, (float)strcmp(person_p->phone, (char *)value));
}

int check_amount_condition(struct Person *person_p, enum operators op, void *value)
{
    return check_condition(op, person_p->current_debt - (*(float *)value));
}

int check_date_condition(struct Person *person_p, enum operators op, void *given_date)
{
    return check_condition(op, date_compare(&person_p->first_trans_date, (struct Date *)given_date));
}

int check_condition(enum operators op, float cmp_res)
{
    switch (op)
    {
    case OP_LT:
        return cmp_res < 0;
    case OP_LE:
        return cmp_res <= 0;
    case OP_EQ:
        return cmp_res == 0;
    case OP_NE:
        return cmp_res != 0;
    case OP_GE:
        return cmp_res >= 0;
    case OP_GT:
        return cmp_res > 0;
    }
}
