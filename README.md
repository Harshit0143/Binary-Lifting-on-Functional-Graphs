# Binary Lifting on functional Graphs
* Solution to CSES problem Planet Queries 2: https://cses.fi/problemset/task/1160
* We are implementing ```Binary Lifting``` on a graph given that each vertex has an ```out-degree``` exactly 1. 
* Thereby, we'll implement other functions like ```kth ancestor``` and ```distance``` between nodes.  
* See my video https://youtu.be/3jj820OMTLk for the underlying Intuition and other insights into the problem,

## Pre-requisites:
1) Graph Basics: DFS/ BFS/ Cycle Detection etc
2) ```Binary Lifting```
3) Disjoint Set union (not mandatory. I have given enough description)

 UPD: There's a cleaner way to scrape off the trees from the cycles. Apply the Kahn's algorithm to it and you'll just be left with the disjoint cycles
