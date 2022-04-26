#include "general_defs.h"
#include "general_functions.h"
#include "data_structure_build.h"
#include "validation.h"
#include "sorting.h"
#include "queries.h"


FILE *try_to_open_the_file();
int is_empty_file(FILE *fp);
int load_from_file(FILE *fp, struct Person **head_p_p);
