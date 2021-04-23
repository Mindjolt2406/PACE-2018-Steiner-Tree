#include<bits/stdc++.h>
#include "Partition.hpp"

template <typename T> ostream& operator<<(ostream& os, const set<T>& s) {os << "{"; for(auto it : s) {if(it != *s.rbegin()) os << it << ", "; else os << it;} os << "}"; return os;}
template <typename T> ostream& operator<<(ostream& os, const vector<T>& v) { os << "["; for (int i = 0; i < v.size(); ++i) { os << v[i]; ;if (i != v.size() - 1) os << ", "; } os << "]"; return os; } 
template<class A, class B> ostream& operator<<(ostream& out, const pair<A, B> &a){ return out<<"("<<a.first<<", "<<a.second<<")";}

class DSU {
    const static int N = 10;
    public:
    int p[N] = {0}, sz[N] = {0};

    void create(int x){
        p[x] = x;
        sz[x] = 1;
        return;
    }

    int find(int x){
    if(x == p[x]) return x;
    return p[x] = find(p[x]);
    }

    void merge(int x, int y){
        x = find(x), y = find(y);
        if(x == y) return;
    if(sz[x] < sz[y]) swap(x, y);
    p[y] = x;
    sz[x] += sz[y];
    return;
    }
};

void Partition::recur(vector<vector<vector<set<int> > > >&allPartitions, int i, int n) {
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


// void Partition::dfs(int u, int key, vector<vector<int> > &adj, vector<int> &vis) {
//     vis[u] = key;
//     for(auto child : adj[u]) {
//         if(!vis[child]) {
//             dfs(child, key, adj, vis);
//         }
//     }
// }

// void Partition::getFinalMergedSetFromEdges(vector<pair<int, int> > &allEdges, vector<set<int> > &finalMergedSet, int n) {
//     vector<vector<int> > adj(n);
//     for(auto edge : allEdges) {
//         adj[edge.first].push_back(edge.second);
//         adj[edge.second].push_back(edge.first);
//     }

//     vector<int> vis(n);
//     int key = 1;
//     for(int i = 0; i < n; i++) {
//         if(!vis[i]) {
//             dfs(i, key, adj, vis);
//             key++;
//         }
//     }

//     finalMergedSet.resize(key-1);
//     for(int i = 0; i < n; i++) {
//         finalMergedSet[vis[i]-1].insert(i);
//     }

//     sort(finalMergedSet.begin(), finalMergedSet.end());
// }

// bool Partition::checkMerge(vector<set<int> > &partLeft, vector<set<int> > &partRight, vector<set<int> > &finalMergedSet, int n) {
//     set<pair<int, int> > edgesLeft, edgesRight;

//     for(auto childSet : partLeft) {
//         vector<int> childList(childSet.begin(), childSet.end());
//         for(int i = 0; i < childList.size(); i++) {
//             for(int j = i+1; j < childList.size(); j++) {
//                 edgesLeft.insert(make_pair(childList[i], childList[j]));
//             }
//         }
//     }

//     for(auto childSet : partRight) {
//         vector<int> childList(childSet.begin(), childSet.end());
//         for(int i = 0; i < childList.size(); i++) {
//             for(int j = i+1; j < childList.size(); j++) {
//                 edgesRight.insert(make_pair(childList[i], childList[j]));
//             }
//         }
//     }

//     for(auto edge : edgesLeft) {
//         if(edgesRight.count(edge)) {
//             return false;
//         }
//     }

//     for(auto edge : edgesRight) {
//         edgesLeft.insert(edge);
//     }

//     vector<pair<int, int> > allEdges(edgesLeft.begin(), edgesLeft.end());
//     getFinalMergedSetFromEdges(allEdges, finalMergedSet, n);
//     return true;
// }

bool Partition::checkMerge(vector<set<int> > &partLeft, vector<set<int> > &partRight, vector<set<int> > &finalMergedSet, int n) {
    DSU dsu;
    for(int i = 0; i < n; i++) dsu.create(i);

    for(auto block : partLeft) {
        vector<int> blockList(block.begin(), block.end());

        for(int i = 1; i < blockList.size(); i++) {
            dsu.merge(blockList[i], blockList[i-1]);
        }
    }

    // Check if connected previously
    for(auto block : partRight) {
        vector<int> blockList(block.begin(), block.end());
        set<int> blockUnique;

        for(auto it : block) {
            blockUnique.insert(dsu.find(it));
        }

        if(blockUnique.size() != blockList.size()) return false;
    }

    // Create merged set
    for(auto block : partRight) {
        vector<int> blockList(block.begin(), block.end());

        for(int i = 1; i < blockList.size(); i++) {
            dsu.merge(blockList[i], blockList[i-1]);
        }
    }

    vector<set<int> > mergeSet(n);
    for(int i = 0; i < n; i++) {
        mergeSet[dsu.find(i)].insert(i);
    }

    for(auto block : mergeSet) {
        if(!block.empty()) {
            finalMergedSet.push_back(block);
        }
    }

    sort(finalMergedSet.begin(), finalMergedSet.end());
    return true;
}

void Partition::getMergedPairMap(vector<vector<set<int> > > &partitionMap, 
    map<vector<set<int> >, int > &inversePartitionMap, 
    map<int, vector<pair<int, int> > >&finalMap, int n) {

    for(int i = 0; i < partitionMap.size(); i++) {
        cout << "i: " << i << endl;
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

vector<set<int> > applyMap(vector<set<int> > &part, vector<int> &allEle) {
    vector<set<int> > v;
    for(auto block : part) {
        set<int> temp;
        for(auto it : block) {
            temp.insert(allEle[it]);
        }
        v.push_back(temp);
    }
    return v;
}

vector<pair<vector<set<int> >,vector< set<int> > > > Partition::permutationMapper(set<set<int> > &partition) {
    vector<int> allElements; // Also a reverse map
    for(auto block : partition) {
        vector<int> tempEle(block.begin(), block.end());
        allElements.insert(allElements.end(), tempEle.begin(), tempEle.end());
    }

    sort(allElements.begin(), allElements.end());

    map<int, int> eleMapper;
    for(int i = 0; i < allElements.size(); i++) {
        eleMapper[allElements[i]] = i;
    }

    vector<set<int> > mappedPartition;
    for(auto block : partition) {
        set<int> mappedBlock;
        for(auto it : block) {
            mappedBlock.insert(eleMapper[it]);
        }
        mappedPartition.push_back(mappedBlock);
    }

    sort(mappedPartition.begin(), mappedPartition.end());

    int indexPartition = inversePartitionMap[mappedPartition];

    vector<pair<int, int> > partitionPairs = partitionPairMap[indexPartition];

    vector<pair<vector<set<int> >,vector< set<int> > > > finalPairPartition;

    for(auto pairPart : partitionPairs) {
        vector<set<int> > part1 = allPartitions[pairPart.first];
        vector<set<int> > part2 = allPartitions[pairPart.second];
        vector<set<int> > final1 = applyMap(part1, allElements);
        vector<set<int> > final2 = applyMap(part2, allElements);

        finalPairPartition.push_back(make_pair(final1, final2));
    }

    return finalPairPartition;
}


void Partition::precomputeMaps(int n) {
    vector<vector<vector<set<int> > > >partitionMaps;
    recur(partitionMaps, 0, n);

    cout << "Partition precompute(): Created Partitions" << endl;

    int partitionIndex = 0;
    int nodeCount = 1;
    for(auto partitionMap : partitionMaps) {
        cout << "Partition precompute(): nodeCount: " << nodeCount << endl;
        sort(partitionMap.begin(), partitionMap.end());
        for(auto partition : partitionMap) {
            allPartitions.push_back(partition);
            inversePartitionMap[partition] = partitionIndex++;
        }
        // for(auto it : partitionMap) t(it);
        // t(partitionMap.size());
        
        getMergedPairMap(partitionMap, inversePartitionMap, partitionPairMap, nodeCount);
        nodeCount++;
    }

    cout << "Partition precompute(): Created Partition Map" << endl;
    // prettyPrintMap();
}

void Partition::prettyPrintMap() {
    for(auto it : partitionPairMap) {
        auto mergedSet = allPartitions[it.first];
        // cerr << mergedSet << ": " << endl;
        cerr << it.second.size() << endl;
        // for(auto pairSet : it.second) {
        //     cerr << "(" << allPartitions[pairSet.first] << ", " << allPartitions[pairSet.second] << ") ";
        // }
        // cerr << endl;
        // cerr << "------" << endl;
    }
}

vector<vector<set<int> > > Partition::allPartitions;
map<vector<set<int> > , int> Partition::inversePartitionMap;
map<int, vector<pair<int, int> > > Partition::partitionPairMap;

/*
1
1
1
4
1
1
1
4
1
1
2
4
30
4
1
2
4
1
2

[{0, 1}]: 
([{0}, {1}], [{0, 1}]) 
------
[{0}, {1, 2}]: 
([{0}, {1}, {2}], [{0}, {1, 2}]) 
------
[{0, 1}, {2}]: 
([{0}, {1}, {2}], [{0, 1}, {2}]) 
------
[{0, 1, 2}]: 
([{0}, {1}, {2}], [{0, 1, 2}]) ([{0}, {1, 2}], [{0, 1}, {2}]) ([{0}, {1, 2}], [{0, 2}, {1}]) ([{0, 1}, {2}], [{0, 2}, {1}]) 
------
[{0, 2}, {1}]: 
([{0}, {1}, {2}], [{0, 2}, {1}]) 
------

*/