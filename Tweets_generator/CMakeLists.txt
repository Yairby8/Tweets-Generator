cmake_minimum_required(VERSION 3.16)
project(ex3 C)

set(CMAKE_C_STANDARD 99)

add_executable(ex3 tweets_generator.c
        tweets_generator.h
        markov_chain.c
        markov_chain.h
        linked_list.c
        linked_list.h
        justdoit_tweets.txt)
