#include "main.h"

int main(int argc, char const *argv[])
{
    int ret_val = RESULT_SUCCESS;
    FILE *fp_debts;
    int load_file_res;
    struct Person *persons_head_p = NULL;

    // no file name argument
    if (argc < 2)
        printf("usage: %s <csv file name>\ntrying to open the default file name '%s'\n",
               argv[0], DEFAULT_FILE_NAME);

    // pass the file name argument if got it, else the default file name
    fp_debts = try_to_open_the_file(argc > 1 ? argv[1] : DEFAULT_FILE_NAME);
    if (fp_debts == NULL)
        return RESULT_ERROR;
    
    do
    {
        // check if the file is empty
        if (is_empty_file(fp_debts))
        {
            fputs("the file is empty.\n", stdout);
        }
        else // the file is not empty, read it
        {
            fputs("loading data from file...\n", stdout);
            load_file_res = load_from_file(fp_debts, &persons_head_p);
            if (load_file_res == EXIT_SIGNAL) // malloc faild and the user want to exit
            {
                ret_val = RESULT_ERROR;
                break;
            }
            printf("%d valid records loaded from the file.\n", load_file_res);
            merge_sort(&persons_head_p);
            print_persons_records(persons_head_p);
        }
        if (prompt(&persons_head_p, fp_debts) == EXIT_SIGNAL)
            ret_val = RESULT_ERROR; // malloc faild and the user want to exit
    } while (0);

    free_person_linked_list(persons_head_p);
    fclose(fp_debts);

    return ret_val;
}
