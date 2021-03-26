#include "NiceTreeDecomposition/NiceTree.hpp"
// #include<bits/stdc++.h>
// using namespace std;

void readGraph(int &numNodes, vector<vector<int> > &adjNodes, map<pair<int, int>, int> &edgeWeight) {
    string sectionName, graphName, nodesName, edgesName;
    int numEdges;
    cin >> sectionName >> graphName >> nodesName >> numNodes >> edgesName >> numEdges;
    adjNodes.resize(numNodes);
    for(int i = 0; i < numEdges; i++) {
        string eName;
        cin >> eName;

        int u, v, w;
        cin >> u >> v >> w;
        u--; v--;

        adjNodes[u].push_back(v);
        adjNodes[v].push_back(u);

        edgeWeight[make_pair(u, v)] = edgeWeight[make_pair(v, u)] = w;
    }
}   

int main() {
    __;
    int numNodes;
    vector<vector<int> > adjNodes;
    map<pair<int, int>, int> edgeWeight;
    readGraph(numNodes, adjNodes, edgeWeight);
    NiceTreeNode::adjNodes = adjNodes;
    NiceTreeNode::readInput(numNodes);
    return 0;
}
