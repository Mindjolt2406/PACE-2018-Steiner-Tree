#include<bits/stdc++.h>
// g++ -std=c++17 -Wl,-stack_size -Wl,0x10000000 main.cpp
#define mt make_tuple
#define mp make_pair
#define pu push_back
#define INF 1000000001
#define MOD 1000000007
#define EPS 1e-6
#define ll long long int
#define ld long double
#define fi first
#define se second
#define all(v) v.begin(),v.end()
#define pr(v) { for(int i=0;i<v.size();i++) { v[i]==INF? cout<<"INF " : cout<<v[i]<<" "; } cout<<endl;}
#define t1(x)                cerr<<#x<<" : "<<x<<endl
#define t2(x, y)             cerr<<#x<<" : "<<x<<" "<<#y<<" : "<<y<<endl
#define t3(x, y, z)          cerr<<#x<<" : " <<x<<" "<<#y<<" : "<<y<<" "<<#z<<" : "<<z<<endl
#define t4(a,b,c,d)          cerr<<#a<<" : "<<a<<" "<<#b<<" : "<<b<<" "<<#c<<" : "<<c<<" "<<#d<<" : "<<d<<endl
#define t5(a,b,c,d,e)          cerr<<#a<<" : "<<a<<" "<<#b<<" : "<<b<<" "<<#c<<" : "<<c<<" "<<#d<<" : "<<d<<" "<<#e<<" : "<<e<<endl
#define t6(a,b,c,d,e,f)          cerr<<#a<<" : "<<a<<" "<<#b<<" : "<<b<<" "<<#c<<" : "<<c<<" "<<#d<<" : "<<d<<" "<<#e<<" : "<<e<<" "<<#f<<" : "<<f<<endl
#define GET_MACRO(_1,_2,_3,_4,_5,_6,NAME,...) NAME
#define t(...) GET_MACRO(__VA_ARGS__,t6,t5, t4, t3, t2, t1)(__VA_ARGS__)
#define _ cerr<<"here"<<endl;
#define __ {ios::sync_with_stdio(false);cin.tie(NULL);cout.tie(NULL);}

using namespace std;
mt19937 rng(chrono::steady_clock::now().time_since_epoch().count());
template <typename T> ostream& operator<<(ostream& os, const set<T>& s) {os << "{"; for(auto it : s) {if(it != *s.rbegin()) os << it << ", "; else os << it;} os << "}"; return os;}
template <int> ostream& operator<<(ostream& os, const vector<int>& v) { os << "["; for (int i = 0; i < v.size(); ++i) { if(v[i]!=INF) os << v[i]; else os << "INF";if (i != v.size() - 1) os << ", "; } os << "]"; return os; } 
template <typename T> ostream& operator<<(ostream& os, const vector<T>& v) { os << "["; for (int i = 0; i < v.size(); ++i) { os << v[i]; ;if (i != v.size() - 1) os << ", "; } os << "]"; return os; } 
template<class A, class B> ostream& operator<<(ostream& out, const pair<A, B> &a){ return out<<"("<<a.first<<", "<<a.second<<")";}

void recur(vector<vector<vector<set<int> > > >&allPartitions, int i, int n) {
    if(i == 0) {
        set<int> s;
        s.insert(0);
        vector<set<int> > t;
        t.push_back(s);
        vector<vector<set<int> > > tempPartitions;
        tempPartitions.push_back(t);
        allPartitions.push_back(tempPartitions);
        recur(allPartitions, i+1, n);
        return;
    }
    if(i == n) return;

    vector<vector<set<int> > > tempPartitions;
    for(auto partition : allPartitions.back()) {
        for(int j = 0; j < partition.size(); j++) {
            partition[j].insert(i);
            tempPartitions.push_back(partition);
            partition[j].erase(i);
        }

        set<int> tempAloneSet;
        tempAloneSet.insert(i);
        partition.push_back(tempAloneSet);
        tempPartitions.push_back(partition);
    }

    allPartitions.push_back(tempPartitions);

    recur(allPartitions, i+1, n);
} 

void dfs(int u, int key, vector<vector<int> > &adj, vector<int> &vis) {
    vis[u] = key;
    for(auto child : adj[u]) {
        if(!vis[child]) {
            dfs(child, key, adj, vis);
        }
    }
}

void getFinalMergedSetFromEdges(vector<pair<int, int> > &allEdges, vector<set<int> > &finalMergedSet, int n) {
    vector<vector<int> > adj(n);
    for(auto edge : allEdges) {
        adj[edge.first].push_back(edge.second);
        adj[edge.second].push_back(edge.first);
    }

    vector<int> vis(n);
    int key = 1;
    for(int i = 0; i < n; i++) {
        if(!vis[i]) {
            dfs(i, key, adj, vis);
            key++;
        }
    }

    finalMergedSet.resize(key-1);
    for(int i = 0; i < n; i++) {
        finalMergedSet[vis[i]-1].insert(i);
    }

    sort(finalMergedSet.begin(), finalMergedSet.end());
}

bool checkMerge(vector<set<int> > &partLeft, vector<set<int> > &partRight, vector<set<int> > &finalMergedSet, int n) {
    set<pair<int, int> > edgesLeft, edgesRight;

    for(auto childSet : partLeft) {
        vector<int> childList(childSet.begin(), childSet.end());
        for(int i = 0; i < childList.size(); i++) {
            for(int j = i+1; j < childList.size(); j++) {
                edgesLeft.insert(make_pair(childList[i], childList[j]));
            }
        }
    }

    for(auto childSet : partRight) {
        vector<int> childList(childSet.begin(), childSet.end());
        for(int i = 0; i < childList.size(); i++) {
            for(int j = i+1; j < childList.size(); j++) {
                edgesRight.insert(make_pair(childList[i], childList[j]));
            }
        }
    }

    for(auto edge : edgesLeft) {
        if(edgesRight.count(edge)) {
            return false;
        }
    }

    for(auto edge : edgesRight) {
        edgesLeft.insert(edge);
    }

    vector<pair<int, int> > allEdges(edgesLeft.begin(), edgesLeft.end());
    getFinalMergedSetFromEdges(allEdges, finalMergedSet, n);
    return true;
}

void getMergedPairMap(vector<vector<set<int> > > &partitionMap, 
    map<vector<set<int> >, int > &inversePartitionMap, 
    map<int, vector<pair<int, int> > >&finalMap, int n) {

    for(int i = 0; i < partitionMap.size(); i++) {
        int partitionIndexI = inversePartitionMap[partitionMap[i]];
        for(int j = i+1; j < partitionMap.size(); j++) {
            int partitionIndexJ = inversePartitionMap[partitionMap[j]];
            vector<set<int> > finalMergedSet;
            if(checkMerge(partitionMap[i], partitionMap[j], finalMergedSet, n)) {
                int partitionIndex = inversePartitionMap[finalMergedSet];
                finalMap[partitionIndex].push_back(make_pair(partitionIndexI, partitionIndexJ));
            }
        }   
    }
}

int main() {
    __;
    int n;
    cin >> n;
    vector<vector<vector<set<int> > > >partitionMaps;
    map<vector<set<int> > , int> inversePartitionMap;
    vector<vector<set<int> > > allPartitions;
    recur(partitionMaps, 0, n);

    map<int, vector<pair<int, int> > > finalMap;

    int partitionIndex = 0;
    int nodeCount = 1;
    for(auto partitionMap : partitionMaps) {
        sort(partitionMap.begin(), partitionMap.end());
        for(auto partition : partitionMap) {
            allPartitions.push_back(partition);
            inversePartitionMap[partition] = partitionIndex++;
        }
        // for(auto it : partitionMap) t(it);
        // t(partitionMap.size());
        
        getMergedPairMap(partitionMap, inversePartitionMap, finalMap, nodeCount);
        nodeCount++;
    }

    for(auto it : finalMap) {
        auto mergedSet = allPartitions[it.first];
        cerr << mergedSet << ": " << endl;
        for(auto pairSet : it.second) {
            cerr << "(" << allPartitions[pairSet.fi] << ", " << allPartitions[pairSet.se] << ") ";
        }
        cerr << endl;
        cerr << "------" << endl;
        // cerr << "[";
        // for(auto it2 : it.se) cerr << it2 << ", "; cerr << "]\n";
    }


    return 0;
}
