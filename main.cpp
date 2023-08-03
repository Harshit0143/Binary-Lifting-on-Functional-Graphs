#include <iostream>
#include <iomanip>
#include <algorithm>
#include <vector>
#include <utility>
#include <set>
#include <unordered_set>
#include <list>
#include <iterator>
#include <deque>
#include <queue>
#include <stack>
#include <set>
#include <bitset>
#include <random>
#include <map>
#include <unordered_map>
#include <stdio.h>
#include <complex>
#include <math.h>
#include <cstring>
#include <chrono>
#include <string>

// ################################################################################################
// ################################################################################################

using namespace std;
#define ll long long
#define pb push_back
#define pob pop_back()
#define mp make_pair
#define fastio                      \
  ios_base::sync_with_stdio(false); \
  cin.tie(NULL)
#define pf push_front
#define pof pop_front()
#define mod 1000000007
#define add_m(a, b) (((a % mod) + (b % mod)) % mod)
#define sub_m(a, b) (((a % mod) - (b % mod) + mod) % mod)
#define mul_m(a, b) (((a % mod) * (b % mod)) % mod)

/*
Notice that there can't be a directed path from 1 vertex in 1 cycle to another verterx in another Different cycle
(because out degree of each cycle is 1)
and then there can be "trees" entering ll o these cycles thorigh 1 vertex only
So we basically have a tree . On top of the tree, instead of a root (which we useally have, we have a cyvle)
Binary lifting on the cycle is easy
Binary lifting on the tree given that on the cycke is easy
How do we differetialte beterrn a tree ad a cyccle?
In one dfs, we can cleanly detect all cycles
You need the "reversed adjacency" graph
Once the cycle is done, you do the same on the "out going trees" using DP. Do it in the BFS order so that the answer
of parent will always be ready on reaching a cildren


We need to assign an id to each cycle and a sub id to eeach tree connected to the cycle

*/

#define MOD 1000000007
int fastExpo(int base, int pow)
{
  int ans = 1;
  while (pow >= 1)
  {
    if (pow % 2 == 0)
    {
      base = (1ll * base * base) % MOD;
      pow /= 2;
    }
    else
    {
      ans = (1ll * ans * base) % MOD;
      --pow;
    }
  }
  return ans;
}

class binary_lift
{
#define LOG 30 // add the LOG optimisation later
private:
  int n;
  vector<vector<int>> dpAncestor;
  vector<int> cycle_sizes;
  vector<int> indegree;
  vector<int> cycle_id;
  vector<int> exp_2;
  vector<int> next__;
  vector<int> depth;
  vector<int> root;
  vector<int> vertex_rank_in_cycle;
  vector<vector<int>> in_vertices;
  int cycle_cnt = 0;

  void show_dp()
  {
    for (int vertex = 0; vertex < n; vertex++)
    {
      cout << vertex << " -> ";
      for (int jump = 0; jump < LOG; jump++)
        cout << dpAncestor[jump][vertex] << ' ';
      cout << '\n';
    }
  }
  void __populate_tree_ancestors__(int parent)
  {
    for (int &child : in_vertices[parent])
    {
      if (root[child] == child) // comp[nbr]== true means it us the part of a cycle. There will be
        continue;               // exactly 1 such node like this

      root[child] = root[parent];
      depth[child] = depth[parent] + 1;
      dpAncestor[0][child] = parent;
      for (int jump = 1; jump < LOG; jump++)
        dpAncestor[jump][child] = dpAncestor[jump - 1][dpAncestor[jump - 1][child]];
      __populate_tree_ancestors__(child);
    }
  }
  void __populate_cycle_ancestors__(vector<int> &cycle)
  {
    int cycle_size = cycle.size();
    for (int rank = 0; rank < (int)cycle.size(); rank++)
    {
      int vertex = cycle[rank];
      vertex_rank_in_cycle[vertex] = rank;
      for (int jump = 0; jump < LOG; jump++)
        dpAncestor[jump][vertex] = cycle[(rank + exp_2[jump]) % cycle_size];
    }
  }

  void __populate_2_exponents__()
  {
    exp_2.resize(LOG);
    for (int i = 0; i < LOG; i++)
      exp_2[i] = fastExpo(2, i);
  }

  void __mark_cycle_id__()
  {

    depth.resize(n);
    vertex_rank_in_cycle.resize(n);
    dpAncestor.resize(LOG, vector<int>(n));

    cycle_id.resize(n, -1);
    for (int vertex = 0; vertex < n; vertex++)
      if (root[vertex] == -1 && cycle_id[vertex] == -1)
      {
        int curr_vertex = vertex;
        vector<int> cycle;
        do
        {
          cycle.push_back(curr_vertex);
          cycle_id[curr_vertex] = cycle_cnt;
          root[curr_vertex] = curr_vertex;
          curr_vertex = next__[curr_vertex];

        } while (curr_vertex != vertex);
        cycle_sizes.push_back(cycle.size());

        ++cycle_cnt;

        __populate_cycle_ancestors__(cycle);
        for (int &parent : cycle)
          __populate_tree_ancestors__(parent);
      }
  }
  void describe_graph()
  {

    cout << "Graph size: " << n << '\n';
    cout << "Printing next__[]\n";
    for (int &out : next__)
      cout << out << ' ';
    cout << "\nPrinting in-adjacecny\n";

    for (int vertex = 0; vertex < n; vertex++)
    {
      cout << vertex << " -> ";
      for (int &in_vertex : in_vertices[vertex])
        cout << in_vertex << ' ';
      cout << '\n';
    }
    cout << "Printing in-degree count\n";
    for (int &indeg : indegree)
      cout << indeg << ' ';

    cout << "\nDone printing\n";
  }

  void __scrape_trees__()
  {

    root.resize(n, -1);
    queue<int> q;
    for (int vertex = 0; vertex < n; vertex++)
      if (!indegree[vertex])
        q.push(vertex);
    while (!q.empty())
    {
      int vertex = q.front();
      root[vertex] = -2; // -2 for tree vertices
      q.pop();
      if (!(--indegree[next__[vertex]]))
        q.push(next__[vertex]);
    }
  }

public:
  binary_lift(int n__)
  {
    n = n__;
    __set_next__();
    __scrape_trees__();
    __populate_2_exponents__();
    __mark_cycle_id__();
  }
  void __set_next__()
  {
    next__.resize(n);
    in_vertices.resize(n);
    indegree.resize(n);

    for (int i = 0; i < n; i++)
    {
      int out_vertex;
      cin >> out_vertex;
      --out_vertex;
      next__[i] = out_vertex;
      in_vertices[out_vertex].push_back(i);
      ++indegree[out_vertex];
    }
  }
  int kth_ancestor(int vertex, int k)
  {
    for (int jump = 0; jump < LOG; jump++)
      if ((k >> jump) & 1)
        vertex = dpAncestor[jump][vertex];
    return vertex;
  }
  int min_steps_a_to_b(int a, int b)
  {
    if (cycle_id[root[a]] != cycle_id[root[b]])
      return -1;

    if (root[a] == root[b])
    {
      if (depth[b] > depth[a])
        return -1;
      else
      {
        int diff = depth[a] - depth[b];
        a = kth_ancestor(a, diff);
        return a == b ? diff : -1;
      }
    }
    if (b != root[b])
      return -1;
    int dist = depth[a];
    a = root[a];
    int diff = vertex_rank_in_cycle[b] - vertex_rank_in_cycle[a];
    if (diff > 0)
      dist += diff;
    else
      dist += cycle_sizes[cycle_id[a]] + diff;
    return dist;
  }
};

signed main()
{

  fastio;
  int n, q;
  cin >> n >> q;
  binary_lift tree(n);
  for (int i = 0; i < q; i++)
  {
    int a, b;
    cin >> a >> b;
    --a, --b;
    cout << tree.min_steps_a_to_b(a, b) << '\n';
  }

  return 0;
}
