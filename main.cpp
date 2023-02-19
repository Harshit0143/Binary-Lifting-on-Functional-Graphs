#include "bits/stdc++.h"
// #include "crush.cpp"
using namespace std;
#define ll long long
#define pb push_back
#define pob pop_back()
#define mp make_pair
#define fastio                        \
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

bool *vis;
bool *curr;
ll *root;
ll *prev_;
ll *cycle_id;
ll *depth;
ll *cycle_rank;
vector<ll> cyc_siz;

ll **up;
ll LOG = 32;
void build(ll n)
{
    up = new ll *[n];
    for (ll i = 0; i < n; i++)
        up[i] = new ll[LOG];
}

ll kth_ancestor(ll ver, ll k)
{

    for (ll i = LOG - 1; i >= 0; i--)
    {
        if (((1ll << i) & k) == 0) // in the later case we need a smaller step
            continue;

        ver = up[ver][i];
    }
    return ver;
}

ll fastExpo(ll a, ll n, ll x)
{

    ll ans = 1;

    while (n >= 1)
    {
        if (n % 2 == 0)
        {
            a = (a * a) % x;
            n /= 2;
        }
        else
        {
            ans = (ans * a) % x;
            n--;
        }
    }
    return ans;
}

void lift_cycle(vector<vector<ll> > &adj, ll cyc, ll a)
{
    vector<ll> cycle;

    ll node = a;
    ll i = 0;
    do
    {

        cycle.push_back(node);
        cycle_id[node] = cyc;
        root[node] = node;
        depth[node] = 0;
        cycle_rank[node] = i;
        node = prev_[node];
        i++;
    } while (node != a);

    ll x = cycle.size(), exp_2[LOG];
    cyc_siz.push_back(x);
    for (ll i = 0; i < LOG; i++)
        exp_2[i] = fastExpo(2, i, x);

    for (ll i = 0; i < x; i++)
        for (ll j = 0; j < LOG; j++)
            up[cycle[i]][j] = cycle[(i + exp_2[j]) % x];
}

void dfs(vector<vector<ll> > &adj, ll &cyc, ll ver)
{
    vis[ver] = curr[ver] = true;
    for (ll nbr : adj[ver])
    {
        prev_[nbr] = ver;
        if (!vis[nbr])
            dfs(adj, cyc, nbr);
        else
        {

            if (curr[nbr])
            {
                lift_cycle(adj, cyc, nbr);
                cyc++;
            }
        }
    }
    curr[ver] = false;
}

void dfs_simple(vector<vector<ll> >  &adj, ll ver, ll par = -1)
{

    for (ll nbr : adj[ver])
    {
        if (root[nbr] == nbr) // comp[nbr]== true means it us the part of a cycle. There will be
            continue;         // exactly 1 such node like this
        root[nbr] = root[ver];
        depth[nbr] = depth[ver] + 1;
        up[nbr][0] = ver;
        for (ll j = 1; j < LOG; j++)
            up[nbr][j] = up[up[nbr][j - 1]][j - 1];

        dfs_simple(adj, nbr, ver);
    }
}

ll lca(ll u, ll v)
{

    ll diff = depth[u] - depth[v], dist = 0;

    for (ll i = LOG - 1; i >= 0; i--)
        if (1 & (diff >> i))
        {
            u = up[u][i];
            dist += (1ll << i);
        }

    return u == v ? dist : -1;
}

signed main()
{

    fastio;
    ll n,q, k, u;
    cin >> n>>q;

    vis = new bool[n];
    curr = new bool[n];
    prev_ = new ll[n];
    root = new ll[n];
    depth = new ll[n];
    cycle_id = new ll[n];
    cycle_rank = new ll[n];

    for (ll i = 0; i < n; i++)
    {
        vis[i] = curr[i] = false;
        root[i] = -1;
    }

    build(n);

    vector<vector<ll> > adjT(n);
    for (ll i = 0; i < n; i++)
    {
        cin >> u;
        u--;
        adjT[u].push_back(i);
    }
    // in the trnaspost graph, each node has atmost 1 in-node. So we use the concept of parent

    ll cyc = 0;
    for (ll i = 0; i < n; i++)
    {
        if (vis[i])
            continue;
        dfs(adjT, cyc, i);
    }
    for (ll i = 0; i < n; i++)
        if (root[i] == i)
            dfs_simple(adjT, i); // we are adding the root of a tree

    ll a, b;

    for (ll i = 0; i < q; i++)
    {
        cin >> a >> b;
        a--, b--;
        if (cycle_id[root[a]] != cycle_id[root[b]])
        {
            cout << "-1\n";
        }
        else if (root[a] == root[b])
        {
            if (depth[b] > depth[a])
                cout << "-1\n";
            else
                cout << lca(a, b) << "\n";
        }

        else if (b != root[b])
            cout << "-1\n";

        else
        {
            ll dist = lca(a, root[a]);
            a = root[a];
            ll diff = cycle_rank[b]-cycle_rank[a];
            if (diff>0) dist+=diff;
            else 
                dist+= cyc_siz[cycle_id[a]]+diff;
      
            cout << dist << "\n";
        }
    }

    return 0;
}