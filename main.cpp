#include "bits/stdc++.h"
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
*/
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

    for (ll i = LOG; i >= 0; i--)
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

void lift_cycle(vector<vector<ll> > &adj, vector<ll> &prev, vector<bool> &comp, ll a)
{
    vector<ll> cycle;
    cycle.push_back(a);

    do
    {
        cycle.push_back(prev[cycle.back()]);
        comp[cycle.back()] = true;
    } while (cycle.back() != a);

    cycle.pop_back();
    ll   x = cycle.size(),exp_2[LOG];
    for (ll i = 0; i  < LOG; i++)
        exp_2[i] = fastExpo(2, i, x);


    for (ll i = 0; i < x; i++)
        for (ll j = 0; j < LOG; j++)
            up[cycle[i]][j] = cycle[(i + exp_2[j])%x];
}

void dfs(vector<vector<ll> > &adj, vector<bool> &vis, vector<bool> &curr, vector<ll> &prev, vector<bool> &comp, ll ver)
{
    vis[ver] = curr[ver] = true;
    for (ll nbr : adj[ver])
    {
        prev[nbr] = ver;
        if (!vis[nbr])
            dfs(adj, vis, curr, prev, comp, nbr);
        else
        {

            if (curr[nbr])
                lift_cycle(adj, prev, comp, nbr);
        }
    }
    curr[ver] = false;
}

void dfs_simple(vector<vector<ll> > &adj, vector<bool> &comp, ll ver, ll par = -1)
{

    for (ll nbr : adj[ver])
    {
        if (comp[nbr] || nbr == par) // comp[nbr]== true means it us the part of a cycle. There will be
            continue;                // exactly 1 such node like this

        up[nbr][0] = ver;
        for (ll j = 1; j < LOG; j++)
            up[nbr][j] = up[up[nbr][j - 1]][j - 1];

        dfs_simple(adj, comp, nbr, ver);
    }
}

signed main()
{


    fastio;
    ll n, q,k,u;
    cin >> n >>q;

    build(n);

    vector<vector<ll> > adjT(n);
    for (ll i = 0; i < n; i++)
    {  cin>>u;
        u--;
        adjT[u].push_back(i);
    }
    // in the trnaspost graph, each node has atmost 1 in-node. So we use the concept of parent

    vector<bool> vis(n), curr(n), comp(n);
    vector<ll> prev(n);
    for (ll i = 0; i < n; i++)
    {
        if (vis[i])
            continue;
       
        dfs(adjT, vis, curr, prev, comp, i);
    }
    for (ll i = 0; i < n; i++)
        if (comp[i])
            dfs_simple(adjT, comp, i); // we are adding the root of a tree


    for (ll i = 0 ; i < q ;i++)
    {  cin>>u>>k;
    u--;
    cout<<kth_ancestor(u,k)+1<<"\n";

    }

  


    return 0;
}