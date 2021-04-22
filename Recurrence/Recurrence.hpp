#ifndef __RECURRENCE_
#define __RECURRENCE_

#include<bits/stdc++.h>
#include "../NiceTreeDecomposition/NiceTree.hpp"
#define ll long long int

class Recurrence {
    public: 
    static map<pair<int, pair<set<int>, set<set<int> > > > , ll> dp; 
    static vector<vector<int> > adjNodes;
    static map<pair<int, int>, int> edgeWeight;
    static set<int> terminals;

    void addOneTerminalAllBags(NiceTreeNode* root);

    ll computeDP(NiceTreeNode* root, set<int> &forestSet, set<set<int> > &partition);

    ll computeLeaf(NiceTreeNode* root, set<int> &forestSet, set<set<int> > &partition);
    ll computeIntroduceVertex(NiceTreeNode* root, set<int> forestSet, set<set<int> > partition);
    ll computeIntroduceEdge(NiceTreeNode* root, set<int> &forestSet, set<set<int> > &partition);
    ll computeForgetVertex(NiceTreeNode* root, set<int> forestSet, set<set<int> > partition);
    ll computeJoinNode(NiceTreeNode* root, set<int> &forestSet, set<set<int> > &partition);
    ll computeWeirdNode(NiceTreeNode* root, set<int> &forestSet, set<set<int> > &partition);

    // Helper methods
    bool removeVertexFromPartition(int vertex, set<set<int> > &partition);
    bool edgeInSameBlock(pair<int, int> edge, set<set<int> > &partition, set<int> &tempBlock);
    void buildSetForEdgePartition(vector<int> &tempBlockList, set<int> &blockA, set<int> &blockB, int mask);
};

#endif