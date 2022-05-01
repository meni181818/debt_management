#include "file_io.h"

FILE *try_fopen(const char *file_name, const char *modes)
{
    FILE *fp;
    int user_selection;

    while ((fp = fopen(file_name, modes)) == NULL)
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

int is_empty_file(FILE *fp)
{
    int ch = fgetc(fp);
    ungetc(ch, fp);
    return ch == EOF;
}

int load_from_file(FILE *fp, struct Person **head_p_p)
{
    int valid_lines_loaded = 0;
    size_t lines_count = 1, line_len;
    char line_buf[INPUT_LINE_BUF_SIZE];
    struct Person *new_preson_p;

    if (is_empty_file(fp)) // check if the file is empty
    {
        fputs("the file is empty.\n\n", stdout);
        return 0; // 0 loaded
    }
    fputs("loading data from the file...\n\n", stdout);

    // read line by line until the EOF
    while (fgets(line_buf, INPUT_LINE_BUF_SIZE, fp) != NULL)
    {
        line_len = strnlen(line_buf, INPUT_LINE_BUF_SIZE);

        if (line_buf[line_len - 1] != '\n') // no '\n' at the end => the line is too long
        {
            fprintf(stderr, "error. line no. %lu is too long!\n", lines_count);
            finish_line(fp);
        }
        else // the line is not too long
        {
            RTRIM_NEW_LINE(line_buf, line_len);                     // remove '\n' from the end
            if (validate_line_cols(line_buf, lines_count) == VALID) // valid line format
            {
                switch (create_person_from_line(line_buf, lines_count, &new_preson_p))
                {
                case EXIT_SIGNAL_ERROR: // malloc failed and the user want to exit
                    return EXIT_SIGNAL_ERROR;
                case RESULT_SUCCESS:
                    // if the data of the new person was inserted we get back non-NULL pointer
                    valid_lines_loaded += (insert_or_update_person(head_p_p, new_preson_p, lines_count) != NULL);
                }
            }
        }
        lines_count++;
    }

    printf("%d valid records loaded from the file.\n\n", valid_lines_loaded);

    return valid_lines_loaded;
}
