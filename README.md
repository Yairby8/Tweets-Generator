# Markov Chain Tweets Generator💬

## Overview  
A C program that generates tweets based on a text corpus using Markov Chains, modeling word-to-word probabilities to create new, stylistically similar tweets. Demonstrates natural language processing, dynamic memory management, and pseudo-random generation.

## Features  
- Reads a tweet corpus and stores unique words with follower frequencies in linked lists  
- Generates probabilistic tweets with reproducible randomness using a seed  
- Handles file errors, memory allocations, and command-line argument validation  
- Supports partial or full corpus reading with flexible parameters  
- Prints tweets prefixed with "Tweet X:" and caps length at 20 words

## Skills & Concepts Demonstrated  
- Natural Language Processing (NLP) using Markov Chains  
- C programming: pointers, dynamic memory allocation, string handling  
- Data structures: linked lists for managing word transitions  
- Probabilistic modeling and pseudo-random generation (`srand`, `rand`)  
- File input/output and robust error handling  
- Modular coding with resource cleanup and memory management  
- Command-line argument parsing and validation  

## Usage  
Compile with:  
```bash
gcc -Wall -Wextra -Wvla -std=c99 tweets_generator.c markov_chain.c linked_list.c -o tweets_generator
```
Run with:  
```bash
./tweets_generator <seed> <num_tweets> <corpus_path> [words_to_read]
```
- `<seed>`: unsigned int for random seed  
- `<num_tweets>`: number of tweets to generate  
- `<corpus_path>`: input text file path  
- `[words_to_read]` (optional): limits words read; defaults to entire file  

Example:
```bash
./tweets_generator 454545 30 justdoit_tweets.txt 100
```
## Demo
![Demo](<img width="922" height="270" alt="Screenshot 2025-10-31 144316" src="https://github.com/user-attachments/assets/2f6ec7bb-1c6b-41d4-b99d-1e2df99ecb15" />)
