#pragma GCC optimize("O3")
#pragma GCC target("sse4")
// #pragma GCC optimize("Ofast")
// #pragma GCC target("avx,avx2,fma")
#include <bits/stdc++.h>
using namespace std;
typedef long long ll;
const double PI = acos(-1.0);
#define t1(x) cerr << #x << "=" << x << endl
#define t2(x, y) cerr << #x << "=" << x << " " << #y << "=" << y << endl
#define t3(x, y, z) cerr << #x << "=" << x << " " << #y << "=" << y << " " << #z << "=" << z << endl
#define t4(a, b, c, d) cerr << #a << "=" << a << " " << #b << "=" << b << " " << #c << "=" << c << " " << #d << "=" << d << endl
#define t5(a, b, c, d, e) cerr << #a << "=" << a << " " << #b << "=" << b << " " << #c << "=" << c << " " << #d << "=" << d << " " << #e << "=" << e << endl
#define t6(a, b, c, d, e, f) cerr << #a << "=" << a << " " << #b << "=" << b << " " << #c << "=" << c << " " << #d << "=" << d << " " << #e << "=" << e << " " << #f << "=" << f << endl
#define GET_MACRO(_1, _2, _3, _4, _5, _6, NAME, ...) NAME
#define tr(...)                                    \
    GET_MACRO(__VA_ARGS__, t6, t5, t4, t3, t2, t1) \
    (__VA_ARGS__)
#define __                            \
    freopen("input.txt", "r", stdin); \
    freopen("output.txt", "w", stdout);
#define fastio()             \
    ios::sync_with_stdio(0); \
    cin.tie(0)
#define MEMS(x, t) memset(x, t, sizeof(x));
mt19937 rng(chrono::steady_clock::now().time_since_epoch().count());
/*-------------------------------------------------------------------------------------------------------------------------------------*/
// #define MOD 1000000007
#define endl "\n"
#define int long long
#define inf 1e18
#define ld long double
/*-------------------------------------------------------------------------------------------------------------------------------------*/

/*
n=number of steiner node(the optional intermediate nodes)
k=number of terminal nodes.(all nodes that has to be connected)
dp[i][mask]=minimum cost that the root is i and contains all terminal nodes which are set in mask.
g[i][j]=minimum cost from steiner node i to steiner node j. Run floyd warshall initially for finding g[][]
*/

map<int, int> original_to_steiner;
map<int, int> steiner_to_original;
map<int, int> original_to_terminal;
map<int, int> terminal_to_original;
vector<vector<pair<int, int>>> adj;

vector<vector<int>> dist;
vector<vector<int>> g;
int solve(int n, int k)
{
    assert(k <= 25);
    assert((1 << k) * n * n <= 1e13);
    tr(n, k);
    vector<vector<int>> dp(n, vector<int>(1 << k, inf));
    // tr("here");
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < k; j++)
        {
            dp[i][(1 << j)] = dist[steiner_to_original[i]][terminal_to_original[j]]; // dist from steiner node i to terminal node j, base case for dp.
        }
        dp[i][0] = 0;
    }
    for (int mask = 1; mask < (1 << k); mask++)
    {
        for (int i = 0; i < n; i++)
        {
            for (int ss = mask; ss > 0; ss = (ss - 1) & mask)
            {
                dp[i][mask] = min(dp[i][mask], dp[i][ss] + dp[i][mask - ss]);
            }
            if (dp[i][mask] < inf)
            {
                for (int j = 0; j < n; j++)
                {
                    dp[j][mask] = min(dp[j][mask], dp[i][mask] + g[i][j]);
                }
            }
        }
    }
    int ans = inf;
    for (int i = 0; i < n; i++)
    {
        ans = min(ans, dp[i][(1 << k) - 1]);
    }
    return ans;
}

signed main()
{
    fastio();

    string section, graph;
    cin >> section >> graph;
    assert(section == "SECTION" && graph == "Graph");

    string Nodes;
    int n;
    cin >> Nodes >> n;
    assert(Nodes == "Nodes");

    string edges;
    int m;
    cin >> edges >> m;
    assert(edges == "Edges");

    adj.resize(n + 1);
    for (int i = 0; i < m; i++)
    {
        string E;
        int u, v, w;
        cin >> E >> u >> v >> w;
        assert(E == "E");

        adj[u].push_back({v, w});
        adj[v].push_back({u, w});
    }

    string End;
    cin >> End;
    assert(End == "END");

    string Terminals;
    cin >> section >> Terminals;
    assert(Terminals == "Terminals");
    cin >> Terminals;
    int nTerminals;
    cin >> nTerminals;

    vector<bool> isTerminal(n + 1, false);
    for (int i = 0; i < nTerminals; i++)
    {
        string temp_T;
        cin >> temp_T;
        assert(temp_T == "T");
        int node;
        cin >> node;
        isTerminal[node] = true;
    }

    string Tree_decomp;
    while (getline(cin, Tree_decomp))
    {
        if (Tree_decomp == "EOF")
            break;
    }

    int curTerminal = 0, curSteiner = 0;

    for (int i = 1; i <= n; i++)
    {
        if (isTerminal[i])
        {
            original_to_terminal[i] = curTerminal;
            terminal_to_original[curTerminal] = i;
            curTerminal++;
        }
        original_to_steiner[i] = curSteiner;
        steiner_to_original[curSteiner] = i;
        curSteiner++;
    }

    dist.assign(n + 1, vector<int>(n + 1, inf));

    for (int i = 1; i <= n; i++)
    {
        for (auto x : adj[i])
        {
            dist[i][x.first] = min(dist[i][x.first], x.second);
        }
        dist[i][i] = 0;
    }
    for (int i = 1; i <= n; i++)
    {
        for (int j = 1; j <= n; j++)
        {
            for (int k = 1; k <= n; k++)
            {
                int v = dist[j][i] + dist[i][k];
                if (v < dist[j][k])
                {
                    dist[j][k] = v;
                }
            }
        }
    }

    g.assign(n , vector<int>(n , 0));
    for (int i = 0; i < n ; i++)
    {
        for (int j = 0; j < n ; j++)
        {
            int u = steiner_to_original[i];
            int v = steiner_to_original[j];
            g[i][j] = dist[steiner_to_original[i]][steiner_to_original[j]];
        }
    }
    cout << solve(n , curTerminal);
}