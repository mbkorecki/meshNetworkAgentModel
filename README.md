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
                \   use a population of 80 agents
                 use set-up 1

In general you can run

    ./app x a b c d e f (g h i)

where

    x = set-up to use: 1 - binary, 2 - stochastic, 3 - advanced strategies
    a = number of agents
    b c = size of the world
    d = number of days to simulate
    e = how often the simulation should run
    f = initial ratio of selfish agents
    g = initial ratio of tit-for-tat agents   (used only for set-up 3)
    h = initial ratio of battery-based agents (used only for set-up 3)
    i = initial ratio of hybrid agents        (used only for set-up 3)

For further examples, see the `Makefile`.
You can run `make all-results` to run all experiments -- they will take a long time!
