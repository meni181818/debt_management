#include "file_io.h"

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

int is_empty_file(FILE *fp)
{
    int ch = fgetc(fp);
    ungetc(ch, fp);
    return ch == EOF;
}

// return the number of valid lines that loaded
int load_from_file(FILE *fp, struct Person **head_p_p)
{
    int ret_val, create_person_res;
    size_t lines_count = 1, line_len;
    char line_buf[INPUT_LINE_BUF_SIZE];
    struct Person *new_preson_p;

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
            RTRIM_NEW_LINE(line_buf, line_len); // remove '\n' from the end
            if (validate_line_cols(line_buf, lines_count) == VALID) // valid line format
            {
                switch (create_person_from_line(line_buf, lines_count, &new_preson_p))
                {
                case EXIT_SIGNAL: // malloc faild and the user want to exit
                    return EXIT_SIGNAL;
                case RESULT_SUCCESS:
                    ret_val += (insert_or_update_person(head_p_p, new_preson_p, lines_count) != NULL);
                }
            }
        }
        lines_count++;
    }

    return ret_val;
}
