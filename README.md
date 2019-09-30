# An agent-based simulation of mobile ad hoc networks

## Compilation

    make

## Usage

The following command runs Set-up 1 as described in the article:

    ./app 1  80  50 50  10  100  0.01
           \   \   \ \    \    \     \
            \   \   \ \    \    \     start with 1% selfish agents
             \   \   \ \    \    run each simulation 100 times
              \   \   \ \    simulate ten days
               \   \   use a 50x50 sized grid
                \   \   use a population of 80 agents
                 use set-up 1

In general you can run

    ./app m a b c d e f (g h i)

where

    m = set-up to use: 1 - binary, 2 - stochastic, 3 - advanced strategies
    a = initial number of agents
    b c = dimensions of the world
    d = number of days the simulation will be run for
    e = the number of data points in one run
    f = initial ratio of selfish agents in the population
    g = initial ratio of tit for tat agents (only used in set-up 3)
    h = initial ratio of battery based agents (only used in set-up 3)
    i = initial ratio of hybrid agents (only used in set-up 3)

For further examples, see the `Makefile`.
You can run `make all-results` to run all experiments -- they will take a long time!
