#include "main.h"

int main(int argc, char const *argv[])
{
    int ret_val = EXIT_SUCCESS;
    FILE *fp_debts;
    int load_file_res;
    struct Person *persons_head_p = NULL;

    if (argc < 2) // no file name argument
        printf("usage: %s <csv file name>\ntrying to open the default file name '%s'\n",
               argv[0], DEFAULT_FILE_NAME);

    // pass the file name argument if got it, else the default file name
    fp_debts = try_fopen(argc > 1 ? argv[1] : DEFAULT_FILE_NAME, "r+");
    if (fp_debts == NULL) // failed to fopen() and the user chose to exit
        return EXIT_FAILURE;

    load_file_res = load_from_file(fp_debts, &persons_head_p);
    if (load_file_res == EXIT_SIGNAL_ERROR) // malloc failed and the user want to exit
    {
        ret_val = EXIT_FAILURE;
        goto out_cleanup;
    }

    merge_sort(&persons_head_p);
    print_persons_records(persons_head_p);

    if (prompt(&persons_head_p, fp_debts) == EXIT_SIGNAL_ERROR) // malloc failed and the user want to exit
        ret_val = EXIT_FAILURE;

out_cleanup:
    free_person_linked_list(persons_head_p);
    fclose(fp_debts);

    return ret_val;
}
