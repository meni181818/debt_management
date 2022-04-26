#include "debt_management.h"

int main(int argc, char const *argv[])
{
    FILE *fp_debts;
    size_t valid_lines_loaded;
    struct Person *persons_head_p = NULL;

    // no file name argument
    if (argc < 2)
        printf("usage: %s <csv file name>\ntrying to open the default file name '%s'\n",
                argv[0], DEFAULT_FILE_NAME);

    // pass the file name argument if got it, else the default file name
    fp_debts = try_to_open_the_file(argc > 1 ? argv[1] : DEFAULT_FILE_NAME);
    if (fp_debts == NULL)
        return RESULT_ERROR;

    // check if the file is empty
    if (is_empty_file(fp_debts))
    {
        fputs("the file is empty.\n", stdout);
    }
    else // the file is not empty, read it
    {
        fputs("loading data from file...\n", stdout);
        valid_lines_loaded = load_from_file(fp_debts, &persons_head_p);
        printf("%u valid records loaded from the file.\n", valid_lines_loaded);
        merge_sort(&persons_head_p);
        print_persons_records(persons_head_p);
    }
    // prompt
    prompt(&persons_head_p, fp_debts);
    
    free_person_linked_list(persons_head_p);
    fclose(fp_debts);

    return RESULT_SUCCESS;
}

FILE *try_to_open_the_file(const char *file_name)
{
    FILE *fp;
    int user_selection;

    while ((fp = fopen(file_name, "r+")) == NULL)
    {
        fprintf(stderr, "error. can not open the file \"%s\".\n", file_name);
        user_selection = get_user_selection_1_9(
            "please select:\n1. try again.\n2. create an empty file.\n3. exit.\n", 3);
        if (user_selection == 1) // try again
        {
            continue;
        }
        else if (user_selection == 2)
        {
            fp = fopen(file_name, "a"); // create new if not exists
            fclose(fp);
        }
        else // 2 => exit
        {
            return fp; // return NULL to main => return 1 on main;
        }
    }

    return fp;
}

// 1: empty, 0: not empty
int is_empty_file(FILE *fp)
{
    int ch = fgetc(fp);
    ungetc(ch, fp);
    return ch == EOF;
}

// return the
size_t load_from_file(FILE *fp, struct Person **head_p_p)
{
    size_t valid_lines_loaded = 0, lines_count = 1, line_len;
    char line_buf[INPUT_LINE_BUF_SIZE];
    enum validation_result validation_res;
    struct Person *new_preson_p;

    // read line by line until the EOF
    while (fgets(line_buf, INPUT_LINE_BUF_SIZE, fp) != NULL)
    {
        line_len = strnlen(line_buf, INPUT_LINE_BUF_SIZE);
        
        if (line_buf[line_len - 1] != '\n') // no '\n' at the end => the line is too long
        {
            fprintf(stderr, "error. line no. %u is too long!\n", lines_count);
            finish_line(fp);
        }
        else // the line is not too long
        {
            RTRIM_NEW_LINE(line_buf, line_len); // remove '\n' from the end
            validation_res = validate_line_cols(line_buf, lines_count);
            if (validation_res == VALID) // valid line format
            {
                new_preson_p = create_person_from_line(line_buf, lines_count);
                valid_lines_loaded += (insert_or_update_person(head_p_p, new_preson_p, lines_count) != NULL);
            }
        }
        lines_count++;
    }

    return valid_lines_loaded;
}
