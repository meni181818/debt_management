#include "general_defs.h"
#include "data_structure_build.h"

void merge_sort(struct Person **head_ref);
struct Person *sorted_merge(struct Person *a, struct Person *b);
void front_back_split(struct Person *source, struct Person **front_ref, struct Person **back_ref);

/*
 * reorder one specific person in the linked list.
 * if it's at the end of the list or the next is > to_reorder_p =>
 *      remove to_reorder_p from the list and call insert_person_in_order
 *      to search the insrtion point from the beginning.
 * else if the next is < to_reorder_p => remove to_reorder_p from the list,
 *      search the insertion point with find_insertion_point,
 *      (-start to search from the next) and insert there.
 * else next is == to_reorder_p do nothing.
 */
void reorder_person(struct Person **head_p_p, struct Person *to_reorder_p);

// return the prev to insert after
struct Person *find_insertion_point(struct Person *start_search_p, struct Person *to_insert_p);

// insert to_insert_p at the first place where his->current_debt <= next element->current_debt
void insert_person_in_order(struct Person **head_p_p, struct Person *to_insert_p);
