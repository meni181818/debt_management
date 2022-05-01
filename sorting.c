#include "sorting.h"

// sorts the linked list by changing next pointers (not data)
void merge_sort(struct Person **head_ref)
{
    struct Person *head = *head_ref;
    struct Person *a, *b;

    // base case -- length 0 or 1
    if ((head == NULL) || (head->next_p == NULL))
        return;

    // split head into 'a' and 'b' sublists
    front_back_split(head, &a, &b);

    // recursively sort the sublists
    merge_sort(&a);
    merge_sort(&b);

    // answer = merge the two sorted lists together
    *head_ref = sorted_merge(a, b);
}

struct Person *sorted_merge(struct Person *a, struct Person *b)
{
    struct Person *result = NULL;

    // base cases
    if (a == NULL)
        return (b);
    else if (b == NULL)
        return (a);

    // pick either a or b, and recur
    if (a->current_debt <= b->current_debt)
    {
        result = a;
        result->next_p = sorted_merge(a->next_p, b);
    }
    else
    {
        result = b;
        result->next_p = sorted_merge(a, b->next_p);
    }
    return (result);
}

/*
 * split the nodes of the given list into front and back halves,
 * and return the two lists using the reference parameters.
 * If the length is odd, the extra node should go in the front list.
 * Uses the fast/slow pointer strategy.
 */
void front_back_split(struct Person *source, struct Person **front_ref, struct Person **back_ref)
{
    struct Person *fast, *slow;
    slow = source;
    fast = source->next_p;

    /* Advance 'fast' two nodes, and advance 'slow' one node */
    while (fast != NULL)
    {
        fast = fast->next_p;
        if (fast != NULL)
        {
            slow = slow->next_p;
            fast = fast->next_p;
        }
    }

    // 'slow' is before the midpoint in the list, so split it in two
    // at that point.
    *front_ref = source;
    *back_ref = slow->next_p;
    // 'tail' to split
    slow->next_p = NULL;
}

void reorder_person(struct Person **head_p_p, struct Person *to_reorder_p)
{
    // if next is the tail or next > to_reorder => maybe the right place is before =>
    // remove it and search the right place from the beginning
    if (to_reorder_p->next_p == NULL || to_reorder_p->current_debt < to_reorder_p->next_p->current_debt)
    {
        remove_person_from_the_list(head_p_p, to_reorder_p);
        insert_person_in_order(head_p_p, to_reorder_p);
    }
    // if the next is not NULL and is < to_insert => we know that the right place is after
    else if (to_reorder_p->current_debt > to_reorder_p->next_p->current_debt)
    {
        remove_person_from_the_list(head_p_p, to_reorder_p);
        // find the right place to insert, search from this point and on (not from the start)
        insert_after(find_insertion_point(to_reorder_p->next_p, to_reorder_p), to_reorder_p);
    }
    // else the next == to_insert. no need to move.
}

struct Person *find_insertion_point(struct Person *start_search_p, struct Person *to_insert_p)
{
    while (start_search_p->next_p != NULL && start_search_p->next_p->current_debt < to_insert_p->current_debt)
        start_search_p = start_search_p->next_p;
    return start_search_p;
}

void insert_person_in_order(struct Person **head_p_p, struct Person *to_insert_p)
{
    // empty list or the first element >= to_insert =>
    // the new Person should be before the current head => insert at head
    if (*head_p_p == NULL || (*head_p_p)->current_debt >= to_insert_p->current_debt)
        insert_person_at_head(head_p_p, to_insert_p);
    else // at least 1 element. find the right place to insert
        insert_after(find_insertion_point(*head_p_p, to_insert_p), to_insert_p);
}
