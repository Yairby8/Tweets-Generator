#include "linked_list.h"
#include "markov_chain.h"
#include <stdio.h>  // For printf(), sscanf()
#include <stdlib.h> // For exit(), malloc()
#include <stdbool.h> // for bool
#include <string.h>



int get_random_number(int max_number)
{
    return rand() % max_number;
}

MarkovNode* get_first_random_node(MarkovChain *markov_chain)
{
    int max_num = markov_chain->database->size;
    int num = get_random_number(max_num);
    Node *node = markov_chain->database->first;
    for (int i = 0; i < num; i++)
    {
        node = node->next;
    }
    while (node->data->total_next_words == 0)
    {
        num = get_random_number(max_num);
        node = markov_chain->database->first;
        for (int i = 0; i < num; i++)
        {
            node = node->next;
        }
    }
    return node->data;
}

MarkovNode* get_next_random_node(MarkovNode *state_struct_ptr)
{
    NextNodeCounter* char_frequencies = state_struct_ptr->counter_list;
    int total = state_struct_ptr->total_next_words;
    int i = get_random_number(total);
    if (total == 1)
    {
        return state_struct_ptr->counter_list->markov_node;
    }
    NextNodeCounter *current = char_frequencies;
    while (i >= current->frequency)
    {
        i -= current->frequency;
        current++;
    }
    return current->markov_node;
}

void generate_random_sequence(MarkovChain *markov_chain, MarkovNode *
first_node, int max_length)
{
    MarkovNode * cur_node = first_node;
    if (!first_node)
    {
        cur_node = get_first_random_node(markov_chain);
    }
    for (int i = 0; i < max_length; i++)
    {
        printf("%s ", cur_node->data);
        cur_node = get_next_random_node(cur_node);
        if (cur_node->total_next_words == 0)
        {
            printf("%s\n", cur_node->data);
            return;
        }
    }
}

bool add_node_to_counter_list(MarkovNode *first_node, MarkovNode *second_node)
{
    NextNodeCounter* cur = first_node->counter_list;
    int size = first_node->total_counters;
    if (first_node->total_next_words == 0)
    {
        first_node->counter_list->markov_node = second_node;
        first_node->counter_list->frequency = 1;
        first_node->total_next_words++;
        first_node->total_counters++;
        return true;
    }
    if (!strcmp(cur->markov_node->data, second_node->data))
    {
        cur->frequency++;
        first_node->total_next_words++;
        return true;
    }
    for (int i = 0; i < size - 1; i++)
    {
        if (!strcmp(cur->markov_node->data, second_node->data))
        {
            cur->frequency++;
            first_node->total_next_words++;
            return true;
        }
        cur++;
    }
    NextNodeCounter *temp = realloc(first_node->counter_list,
                                    (size + 1) * sizeof (NextNodeCounter));
    if (!temp)
    {
        return false;
    }
    first_node->counter_list = temp;
    first_node->counter_list [size].markov_node = second_node;
    first_node->counter_list [size].frequency = 1;
    first_node->total_next_words++;
    first_node->total_counters++;
    return true;
}

Node* get_node_from_database(MarkovChain *markov_chain, char *data_ptr)
{
    Node* cur = markov_chain->database->first;
    if (!cur)
    {
        return NULL;
    }
    if (!strcmp(cur->data->data, data_ptr))
    {
        return (cur);
    }
    for (int i = 0; i < markov_chain->database->size - 1; i++)
    {
        cur = cur->next;
        if (!strcmp(cur->data->data, data_ptr))
        {
            return (cur);
        }
    }
    return NULL;
}

Node* add_to_database(MarkovChain *markov_chain, char *data_ptr)
{
    if (get_node_from_database(markov_chain, data_ptr))
    {
        return get_node_from_database(markov_chain, data_ptr);
    }
    MarkovNode* markov_node = initialize_markov_node(data_ptr);
    if (!markov_node)
    {
        return NULL;
    }
    add(markov_chain->database, markov_node);
    return markov_chain->database->last;
}

MarkovNode *initialize_markov_node(const char *data_ptr)
{
    MarkovNode* markov_node = malloc(sizeof (MarkovNode));
    if (!markov_node)
    {
        return NULL;
    }
    markov_node->counter_list = malloc(sizeof (NextNodeCounter));
    if (!markov_node->counter_list)
    {
        free(markov_node);
        markov_node = NULL;
        return NULL;
    }
    markov_node->counter_list->markov_node = NULL;
    markov_node->counter_list->frequency = 0;
    markov_node->data = malloc(strlen(data_ptr) + 1);
    if (!markov_node->data)
    {
        free(markov_node->counter_list);
        free(markov_node);
        markov_node = NULL;
        return NULL;
    }
    strcpy(markov_node->data, data_ptr);
    markov_node->total_next_words = 0;
    markov_node->total_counters = 0;
    return markov_node;
}

MarkovChain *initialize_markov_chain()
{
    LinkedList* database = malloc(sizeof (LinkedList));
    if (!database)
    {
        return NULL;
    }
    database->first = NULL;
    database->last = NULL;
    database->size = 0;
    MarkovChain* markov_chain = malloc(sizeof (MarkovChain));
    if (!markov_chain)
    {
        free(database);
        database = NULL;
        return NULL;
    }
    markov_chain->database = database;
    return markov_chain;
}


void free_markov_chain(MarkovChain ** ptr_chain)
{
    Node* cur = (*ptr_chain)->database->first;
    for (int i = 0; i < (*ptr_chain)->database->size; i++)
    {
        free(cur->data->counter_list);
        cur->data->counter_list = NULL;
        free(cur->data->data);
        cur->data->data = NULL;
        cur->data->total_next_words = 0;
        free(cur->data);
        cur->data = NULL;
        Node* temp = cur;
        cur = cur->next;
        free(temp);
    }
    free(cur);
    cur = NULL;
    free((*ptr_chain)->database);
    (*ptr_chain)->database = NULL;
    free(*ptr_chain);
    *ptr_chain = NULL;
}
