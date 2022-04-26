#include "sorting.h"

// sorts the linked list by changing next pointers (not data)
void merge_sort(struct Person **head_ref)
{
    struct Person *head = *head_ref;
    struct Person *a;
    struct Person *b;

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
    struct Person *fast;
    struct Person *slow;
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

// insert to_insert_p at the first place where his->current_debt <= next element->current_debt
void insert_person_in_order(struct Person **head_p_p, struct Person *to_insert_p)
{
    struct Person *prev_p, *tmp_p;

    // empty list or the first element >= to_insert => insert at head
    if (*head_p_p == NULL || (*head_p_p)->current_debt >= to_insert_p->current_debt)
    {
        insert_person_at_head(head_p_p, to_insert_p);
    }
    else // at least 1 element. find the right place to insert
    {
        prev_p = *head_p_p;
        tmp_p = prev_p->next_p;

        while (tmp_p != NULL && tmp_p->current_debt < to_insert_p->current_debt)
        {
            prev_p = tmp_p;
            tmp_p = tmp_p->next_p;
        }
        prev_p->next_p = to_insert_p;
        to_insert_p->next_p = tmp_p;
    }
}