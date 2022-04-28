#include "general_defs.h"
#include "general_functions.h"
#include "data_structure_build.h"

/*
 * try to open the .csv file file_name.
 * if fopen failed => ask the user to select:
 *    try again, create an empty file, or exit
 * return: FILE pointer if fopen success,
 *         NULL if failed and the user chose to exit
 */
FILE *try_to_open_the_file(const char *file_name);
// 1: empty, 0: not empty
int is_empty_file(FILE *fp);
/*
 * read the file line by line, validate it,
 * call create_person_from_line() and insert_or_update_person()
 * for creating and inserting the data to the data structure.
 * return: the number of valid lines that loadad,
 *         or EXIT_SIGNAL (-1) if malloc failed and the user chose to exit.
 */
int load_from_file(FILE *fp, struct Person **head_p_p);
