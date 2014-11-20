#! /bin/sh

INSTANCES=$(ls instances/)
PATH="instances/"
RESULTS=".results"
echo $PATH
for i in $INSTANCES;
do
    name=$i
    # Run all knapsack problems and save the results to a file.
    ./coloring $PATH$name 1 > "results/random-small-"$name$RESULTS
    ./coloring $PATH$name 2 > "results/random-big-"$name$RESULTS
    ./coloring $PATH$name 3 > "results/greedy-small-"$name$RESULTS
    ./coloring $PATH$name 4 > "results/greedy-big-"$name$RESULTS
done