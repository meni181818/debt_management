#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#ifndef DEBT_MANAGMENT_GENERAL_DEFS
#define DEBT_MANAGMENT_GENERAL_DEFS

#define DEFAULT_FILE_NAME "debts.csv"
#define PERSON_FIELDS_N 6
#define RESULT_ERROR 1
#define RESULT_SUCCESS 0
#define EXIT_SIGNAL -1
#define INPUT_LINE_BUF_SIZE 256
#define PERSONS_TABLE_HEADER "\nfirst name\tlast name \tid       \tphone     \tcurrent debt\tearliest date\n\n"

// functions-like macros <<< [a, b, c, \n, \0]
#define RTRIM_NEW_LINE(line, len)  \
    do                             \
    {                              \
        if (line[(len)-1] == '\n') \
            line[(len)-1] = '\0';  \
    } while (0)

#define PRINT_PERSON(person_p)                                               \
    printf("%-10s\t%-10s\t%s\t%s\t%12g\t   %02d/%02d/%d\n",                  \
           person_p->first_name, person_p->last_name, person_p->id,          \
           person_p->phone, person_p->current_debt,                          \
           person_p->first_trans_date.day, person_p->first_trans_date.month, \
           person_p->first_trans_date.year);

#define DATE_DELIM_STR "/"

struct Date
{
    unsigned int day : 5;
    unsigned int month : 4;
    unsigned int year : 14; // place for evry 4 digits decimal number (and more)
};

struct Person
{
    char *first_name;
    char *last_name;
    char id[10];
    char phone[11];
    float current_debt;
    struct Date first_trans_date;
    struct Person *next_p;
};

// validation
#define ID_VALID_LEN 9
#define PHONE_VALID_LEN 10
#define PHONE_PREFIX "0"
#define FILE_DELIM_CHAR ','
#define FILE_DELIM_STR ","
#define FILE_COLUMNS 6
// fname, lname, id, phone, amount, date, commas, \n \ EOF
#define MIN_LINE_LEN (1 + 1 + ID_VALID_LEN + PHONE_VALID_LEN + 1 + 8 + FILE_COLUMNS - 1 + 1)
#define MIN_DATE_LEN 8
#define MAX_DATE_LEN 10

#define DATE_DELIM_CHAR '/'
#define DATE_MIN_DAY 1
#define DATE_MAX_DAY 31
#define DATE_MIN_MONTH 1
#define DATE_MAX_MONTH 12
#define DATE_MIN_YEAR 1000
#define DATE_MAX_YEAR 9999

enum validation_result
{
    VALID = 0,
    EMPTY_STRING = -1,
    TOO_SHORT = -2,
    TOO_LONG = -3,
    CONTAINS_INVALID_CHARS = -4,
    INVALID_FORMAT = -5,
    MISSING_COLUMNS = -6,
    TOO_MUCH_COLUMNS = -7,
    NULL_INPUT = -8,
    PROCESSING_FAILED = -9,
};

// queries
enum operators
{
    // less-than, less-equals, equals, not-equals, greater-equals, grater-than
    OP_INVALID_OP = -1,
    OP_LT,
    OP_LE,
    OP_EQ,
    OP_NE,
    OP_GE,
    OP_GT
};

enum person_fields
{
    P_INVALID_FIELD = -1,
    P_FIRST_NAME,
    P_LAST_NAME,
    P_ID,
    P_PHONE,
    P_CURRENT_DEBT,
    P_DATE
};

#endif