#ifndef __PARTITION_
#define __PARTITION_

#include<bits/stdc++.h>
#include<set>

using namespace std;

class Partition {
    public: 
    static vector<vector<set<int> > > allPartitions;
    static map<vector<set<int> > , int> inversePartitionMap;
    static map<int, vector<pair<int, int> > > partitionPairMap;

    static void precomputeMaps(int n); // Max bag size
    static void prettyPrintMap();
    static vector<pair<vector<set<int> >,vector< set<int> > > > permutationMapper(set<set<int> >  &v);

    private:
    // static void dfs(int u, int key, vector<vector<int> > &adj, vector<int> &vis);
    // static void getFinalMergedSetFromEdges(vector<pair<int, int> > &allEdges, vector<set<int> > &finalMergedSet, int n);
    static bool checkMerge(vector<set<int> > &partLeft, vector<set<int> > &partRight, vector<set<int> > &finalMergedSet, int n);
    static void getMergedPairMap(vector<vector<set<int> > > &partitionMap, 
    map<vector<set<int> >, int > &inversePartitionMap, 
    map<int, vector<pair<int, int> > >&finalMap, int n);
    static void recur(vector<vector<vector<set<int> > > >&allPartitions, int i, int n);

    static void addPartitionPairMapToFile();
    static void readPartitionPairMapFromFile();
};

#endif