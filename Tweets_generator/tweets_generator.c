#include <stdio.h>
#include "markov_chain.h"
#include "linked_list.h"
#include "string.h"
#define BUFFER_LENGTH 1000
#define DELIMITERS " \n\r"
#define DECIMAL 10
#define SEED 1
#define NUM_OF_TWEETS 2
#define FILE_PATH 3
#define NUM_OF_WORDS 4
#define ALL_FILE_WORDS 0
#define WITH_WORD_LIMIT 5
#define NO_WORD_LIMIT 4
#define MAX_TWEET_SIZE 20


void initial_fill(FILE *fp, int words_to_read, MarkovChain *markov_chain);

void counters_fill(FILE *fp, int words_to_read, MarkovChain *markov_chain);

void search_for_next_words(FILE *fp, int words_to_read,
                           MarkovChain *markov_chain, Node *cur);

int get_num_of_words(int argc, char *const *argv);

void fill_database (FILE *fp, int words_to_read, MarkovChain
*markov_chain)
{
    initial_fill(fp, words_to_read, markov_chain);
    counters_fill(fp, words_to_read, markov_chain);
}

void counters_fill(FILE *fp, int words_to_read, MarkovChain *markov_chain) {
    Node* cur = markov_chain->database->first;
    for (int i = 0; i < markov_chain->database->size - 1; i++)
    {
        char last = cur->data->data[strlen(cur->data->data) - 1];
        if (last == '.')
        {
            cur = cur->next;
            continue;
        }
        search_for_next_words(fp, words_to_read, markov_chain, cur);
        rewind(fp);
        cur = cur->next;
    }
}

void search_for_next_words(FILE *fp, int words_to_read,
                           MarkovChain *markov_chain, Node *cur)
{
    char input2[BUFFER_LENGTH];
    int words_read = 0;
    bool previous_was_equal = false;
    bool maxed_words = false;
    bool no_limit = (words_to_read == ALL_FILE_WORDS);
    while (fgets (input2, sizeof (input2), fp) != NULL)
    {
        char *token2 = strtok (input2, DELIMITERS);
        while (token2)
        {
            if (words_read >= words_to_read)
            {
                maxed_words = true;
                if (!no_limit)
                {
                    break;
                }
            }
            if (previous_was_equal)
            {
                add_node_to_counter_list(cur->data,
                                         get_node_from_database(
                                                 markov_chain, token2)->data);
                previous_was_equal = false;
            }
            if (!strcmp(cur->data->data, token2))
            {
                previous_was_equal = true;
            }
            words_read ++;
            token2 = strtok (NULL, DELIMITERS);
        }
        if (maxed_words && !no_limit)
        {
            rewind(fp);
            break;
        }
    }
}

void initial_fill(FILE *fp, int words_to_read, MarkovChain *markov_chain)
{
    char input[BUFFER_LENGTH];
    int words_read = 0;
    bool maxed_words = false;
    bool no_limit = (words_to_read == ALL_FILE_WORDS);
    while (fgets (input, sizeof (input), fp) != NULL)
    {
        char *token = strtok (input, DELIMITERS);
        while (token)
        {
            if (words_read >= words_to_read)
            {
                maxed_words = true;
                if (!no_limit)
                {
                    break;
                }
            }
            add_to_database(markov_chain, token);
            words_read ++;
            token = strtok (NULL, DELIMITERS);
        }
        if (maxed_words && !no_limit)
        {
            break;
        }
    }
    rewind(fp);
}


int main (int argc, char *argv[])
{
    if (argc != WITH_WORD_LIMIT && argc != NO_WORD_LIMIT)
    {
        printf("arguments error");
    }
    srand((int) strtol(argv[SEED], NULL, DECIMAL));
    FILE *fp = fopen(argv[FILE_PATH], "r");
    if (fp == NULL)
    {
        printf("cant read file");
        return EXIT_FAILURE;
    }
    int words_to_read = get_num_of_words(argc, argv);
    MarkovChain* m_chain = initialize_markov_chain();
    fill_database(fp, words_to_read, m_chain);
    fclose(fp);
    int num_of_tweets = (int) strtol(argv[NUM_OF_TWEETS], NULL, DECIMAL);
    for (int i = 0; i < num_of_tweets; i++)
    {
        printf("Tweet %d: ", i + 1);
        MarkovNode* first_node = get_first_random_node(m_chain);
        generate_random_sequence(m_chain, first_node, MAX_TWEET_SIZE);
    }
    free_markov_chain(&m_chain);
    return 0;
}

int get_num_of_words(int argc, char *const *argv)
{
    int words_to_read = 0;
    if (argc == NO_WORD_LIMIT)
    {
        words_to_read = ALL_FILE_WORDS;
    }
    if (argc == WITH_WORD_LIMIT)
    {
        words_to_read = (int) strtol(argv[NUM_OF_WORDS], NULL, DECIMAL);
    }
    return words_to_read;
}
