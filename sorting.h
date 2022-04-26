#include "general_defs.h"
#include "data_structure_build.h"

void merge_sort(struct Person **head_ref);
struct Person *sorted_merge(struct Person *a, struct Person *b);
void front_back_split(struct Person *source, struct Person **front_ref, struct Person **back_ref);
void insert_person_in_order(struct Person **head_p_p, struct Person *to_insert_p);
