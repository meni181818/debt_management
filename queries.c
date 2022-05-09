#include "queries.h"

int prompt(struct Person **head_p_p, FILE *fp)
{
    int route_cmd_res;
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

        route_cmd_res = route_cmd(cmd_buf, cmd_len, head_p_p, fp);
        if (route_cmd_res == EXIT_SIGNAL) // 'quit' command
            break;
        if (route_cmd_res == EXIT_SIGNAL_ERROR) // malloc failed and the user want to exit
            return EXIT_SIGNAL_ERROR;
    }
    return RESULT_SUCCESS;
}

int route_cmd(char *cmd_str, size_t cmd_len, struct Person **head_p_p, FILE *fp)
{
    char *tok = strtok(cmd_str, " ");
    if (tok == NULL) // just hit space(s) and enter
        return RESULT_ERROR;

    if (strcmp(tok, "select") == 0)
        return make_select(tok, *head_p_p);

    if (strcmp(tok, "set") == 0)
        return make_set(tok, head_p_p, fp);

    if (strcmp(tok, "print") == 0 && strtok(NULL, " ") == NULL) // && no another word(s)
    {
        print_persons_records(*head_p_p);
        return RESULT_SUCCESS;
    }

    if (strcmp(tok, "quit") == 0 && strtok(NULL, " ") == NULL) // && no another word(s)
        return EXIT_SIGNAL;

    if (strcmp(tok, "help") == 0 && strtok(NULL, " ") == NULL) // && no another word(s)
    {
        fputs(QUIERIES_HELP_TXT, stdout);
        return RESULT_SUCCESS;
    }

    // non of the above => unknown command
    PRINT_INVALID_CMD("unknown command/invalid syntax");

    return RESULT_ERROR;
}

int make_set(char *cmd_tok, struct Person **head_p_p, FILE *fp)
{
    char *fields_p_arr[PERSON_FIELDS_N] = {0}; // zero the arr
    enum person_fields current_field;
    char *value_str_p;
    size_t overall_len = FILE_COLUMNS - 1 + 1; // (5 * ',' + '\0')
    // validation will mutate it so create a copy for writing to the file
    char final_line_for_validation[INPUT_LINE_BUF_SIZE], final_line_for_file[INPUT_LINE_BUF_SIZE];
    struct Person *new_person;

    // we now at the first token ("set"). loop 6 times for all person fields.
    for (size_t i = 0; i < PERSON_FIELDS_N; i++)
    {
        cmd_tok = strtok(NULL, ",");
        if (cmd_tok == NULL) // missing token
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
        cmd_tok = str_strip_in_place(cmd_tok); // strip spaces from the field name
        current_field = parse_field(cmd_tok);
        *value_str_p = '='; // bring it back (-for strtok)

        // check if we got duplicate field names (we zero that array above)
        if (fields_p_arr[current_field] != 0)
        {
            PRINT_INVALID_CMD("duplicate fields");
            return RESULT_ERROR;
        }
        // point it to it's next char (=> the actual value string) and save it on the fields array
        fields_p_arr[current_field] = str_strip_in_place(value_str_p + 1);
        overall_len += strlen(fields_p_arr[current_field]);
    }
    
    // vavidate no more arguments
    if (strtok(NULL, " ,") != NULL)
    {
        PRINT_INVALID_CMD("too much arguments");
        return RESULT_ERROR;
    }

    // create a line like in the file (for person object creation)
    snprintf(final_line_for_validation, INPUT_LINE_BUF_SIZE, "%s,%s,%s,%s,%s,%s",
             fields_p_arr[0], fields_p_arr[1], fields_p_arr[2],
             fields_p_arr[3], fields_p_arr[4], fields_p_arr[5]);
    // copy it before validation (validation using strtok and mutate it)
    strncpy(final_line_for_file, final_line_for_validation, INPUT_LINE_BUF_SIZE);

    switch (create_person_from_line(final_line_for_validation, 0, &new_person))
    {
    case EXIT_SIGNAL_ERROR: // malloc failed and the user want to exit
        return EXIT_SIGNAL_ERROR;
    case RESULT_ERROR:
        return RESULT_ERROR;
    }

    new_person = insert_or_update_person(head_p_p, new_person, 0);
    // conflicting names and id
    if (new_person == NULL)
        return RESULT_ERROR;
    reorder_person(head_p_p, new_person);

    // write the new record line to the file
    fprintf(fp, "%s\n", final_line_for_file);
    return RESULT_SUCCESS;
}

int make_select(char *cmd_tok, struct Person *head_p)
{
    enum person_fields field_type;
    enum operators op;
    char *op_start; // point at the start of the operator sub-string.
    size_t op_len, found_count = 0;
    float given_debt;
    struct Date given_date;
    // pointer to the value to compare
    void *value_p;

    // array of pointers to comparison functions
    static comparison_func_p_t comparison_funcs_arr[PERSON_FIELDS_N] = {
        &check_first_name_condition, &check_last_name_condition,
        &check_id_condition, &check_phone_condition,
        &check_amount_condition, &check_date_condition};

    // we now at the first token ("select"). get the rest.
    cmd_tok = strtok(NULL, "");
    if (cmd_tok == NULL)
    {
        PRINT_INVALID_CMD("missing field");
        return RESULT_ERROR;
    }

    // get the operator
    op_start = strpbrk(cmd_tok, OPERATORS_CHARS);
    if (op_start == NULL)
    {
        PRINT_INVALID_CMD("missing operator");
        return RESULT_ERROR;
    }
    op_len = strspn(op_start, OPERATORS_CHARS);
    op = parse_operator(op_start, op_len);
    if (op == OP_INVALID_OP)
    {
        PRINT_INVALID_CMD("invalid operator");
        return RESULT_ERROR;
    }

    // get the field (-may override the operator)
    cmd_tok = strtok(cmd_tok, " " OPERATORS_CHARS);
    if (cmd_tok == NULL)
    {
        PRINT_INVALID_CMD("missing field name");
        return RESULT_ERROR;
    }
    field_type = parse_field(cmd_tok);
    if (field_type == P_INVALID_FIELD)
    {
        PRINT_INVALID_CMD("invalid field");
        return RESULT_ERROR;
    }

    // move to the value (skip the operator)
    cmd_tok = op_start + op_len;
    // trim spaces from the value
    cmd_tok = str_strip_in_place(cmd_tok);
    
    // validate the value
    if (validate_cmd_value(cmd_tok, field_type) != VALID)
    {
        PRINT_INVALID_CMD("invalid value");
        return RESULT_ERROR;
    }

    // if the field is: first/last name/id/phone: use the cmd_tok as value to compare (string)
    // if the field is: debt/date: convert it to float/Date to use in the comparison function
    switch (field_type)
    {
    case P_CURRENT_DEBT:
        given_debt = atof(cmd_tok);
        value_p = &given_debt;
        break;
    case P_DATE:
        given_date = str_to_date(cmd_tok);
        value_p = &given_date;
        break;
    default: // first/last name, id, phone (-string fields)
        value_p = cmd_tok;
        break;
    }

    // print matching result
    found_count = print_persons_filtered(head_p, op, value_p, comparison_funcs_arr[field_type]);
    // if no result
    if (!found_count)
        fputs("no results found for the query\n", stdout);
    else
        printf("found %lu matching records\n", found_count);
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

enum operators parse_operator(const char *op_str, size_t op_len)
{
    switch (op_len)
    {
    case 1:
        if (strncmp(op_str, "<", op_len) == 0)
            return OP_LT;
        if (strncmp(op_str, "=", op_len) == 0)
            return OP_EQ;
        if (strncmp(op_str, ">", op_len) == 0)
            return OP_GT;
        break;
    case 2:
        if (strncmp(op_str, "<=", op_len) == 0)
            return OP_LE;
        if (strncmp(op_str, "!=", op_len) == 0)
            return OP_NE;
        if (strncmp(op_str, ">=", op_len) == 0)
            return OP_GE;
        break;
    }
    return OP_INVALID_OP;
}

size_t print_persons_filtered(
    struct Person *head_p, enum operators op, void *value,
    comparison_func_p_t comparison_func_p)
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
    return check_condition(op, (float)strcasecmp(person_p->first_name, (char *)value));
}

int check_last_name_condition(struct Person *person_p, enum operators op, void *value)
{
    return check_condition(op, (float)strcasecmp(person_p->last_name, (char *)value));
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
    return check_condition(op, date_compare(&person_p->erliest_date, (struct Date *)given_date));
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
    default:
        return 1; // condition is false
    }
}
