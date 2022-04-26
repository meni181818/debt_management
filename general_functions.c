#include "general_functions.h"

struct Date str_to_date(char *date_str)
{
    struct Date ret;
    do
    {
        // day
        char *tok = strtok(date_str, DATE_DELIM_STR);
        if (tok == NULL)
            break;
        ret.day = atoi(tok);

        // month
        tok = strtok(NULL, DATE_DELIM_STR);
        if (tok == NULL)
            break;
        ret.month = atoi(tok);

        // year
        tok = strtok(NULL, DATE_DELIM_STR);
        if (tok == NULL)
            break;
        ret.year = atoi(tok);

        return ret;
    } while (0);

    // in case of error while parsnig the date (NULL token)
    return (struct Date){0, 0, 0};
}

void print_persons_records(struct Person *head_p)
{
    fputs(PERSONS_TABLE_HEADER, stdout);
    while (head_p != NULL)
    {
        PRINT_PERSON(head_p);
        head_p = head_p->next_p;
    }
    fputc('\n', stdout);
}

/*
 * return like strcmp.
 * 0 if equals,
 * positive value if dt_1_p > dt_2_p,
 * negative value if dt_1_p < dt_2_p
 */
int date_compare(struct Date *dt_1_p, struct Date *dt_2_p)
{
    if (dt_1_p->year != dt_2_p->year)
        return (int)dt_1_p->year - dt_2_p->year;
    if (dt_1_p->month != dt_2_p->month)
        return (int)dt_1_p->month - dt_2_p->month;
    if (dt_1_p->day != dt_2_p->day)
        return (int)dt_1_p->day - dt_2_p->day;
    return 0;
}

int get_user_selection_1_9(const char *description, int options_n)
{
    int is_first = 1;
    int inpt;
    do
    {
        if (!is_first)
            fputs("incorrect selection.\n", stdout);
        is_first = 0;

        fputs(description, stdout);
        inpt = fgetc(stdin);
        // if we left some chars on stdin => clear it
        if (inpt != '\n' || inpt != EOF)
            finish_line(stdin);

    } while (inpt - '0' < 1 || inpt - '0' > options_n);
    return inpt - '0';
}

void finish_line(FILE *fp)
{
    int ch;
    while ((ch = fgetc(fp)) != '\n' && ch != EOF)
        ;
}

/*
 * replace the old sub-str in str_in with new sub-str.
 * both have to be in same length.
 */
char *str_replace_in_place(char *str_in, char *old, char *new)
{
    size_t rep_len = strlen(old);
    char *rep_p;
    if (rep_len != strlen(new))
        return NULL;
    while ((rep_p = strstr(str_in, old)) != NULL)
    {
        strncpy(rep_p, new, rep_len);
        rep_p += rep_len;
    }
    return str_in;
}


