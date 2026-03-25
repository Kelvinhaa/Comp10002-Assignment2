Preferential Voting Simulation
A simulation of preferential (ranked-choice) voting implemented in C.

How it works
Voters rank candidates in order of preference. If no candidate wins an outright majority after the first count, the candidate with the fewest votes is eliminated and their votes are redistributed to the next valid preference on each ballot. Rounds continue until a candidate crosses the majority threshold.

Algorithm
Each round runs a linear scan over all active ballots to tally first-preference votes among remaining candidates. The minimum vote-getter is identified and marked eliminated. Redistribution then walks each affected ballot forward until it finds a non-eliminated candidate or exhausts all preferences. This repeats until a winner emerges, giving an overall time complexity of O(r × n) where r is the number of rounds and n is the number of ballots.

Memory management
Ballots are stored as a linked list, learning data structure!
