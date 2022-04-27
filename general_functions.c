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
        if (is_first)
            is_first = 0;
        else // only after the first try
            fputs("incorrect selection.\n", stdout);

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

void *try_malloc(size_t size)
{
    void *ret;
    int user_selection;

    while ((ret = malloc(size)) == NULL)
    {
        user_selection = get_user_selection_1_9("memory allocation faild. please select:\n1. try again.\n2. exit.", 2);
        if (user_selection == 2)
            break;
    }
    return ret;
}

char *str_strip_in_place(char *str)
{
    size_t len;

    while (*str && isspace(*str))
        str++;
    
    for (len = strlen(str); len > 1 && isspace(str[len - 1]); str[--len] = '\0')
        ;

    return str;
}