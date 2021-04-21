#include "NiceTreeDecomposition/NiceTree.hpp"
#include "Recurrence/Recurrence.hpp"
// #include<bits/stdc++.h>
// using namespace std;

void readGraph(int &numNodes, vector<vector<int> > &adjNodes, map<pair<int, int>, int> &edgeWeight) {
    string sectionName, graphName, nodesName, edgesName, endName;
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
    cin >> endName;
} 

void readTerminals(vector<int> &terminals) {
    string sectionName, terminalName, terminalsName, endName;
    int terminalNum;

    cin >> sectionName >> terminalName >> terminalsName;
    cin >> terminalNum;
    
    for(int i = 0; i < terminalNum; i++) {
        string tName;
        cin >> tName;

        int u;
        cin >> u;
        u--;

        terminals.push_back(u);
    }

    cin >> endName;
}

int main() {
    __;
    int numNodes;
    vector<vector<int> > adjNodes;
    map<pair<int, int>, int> edgeWeight;
    vector<int> terminals;

    readGraph(numNodes, adjNodes, edgeWeight);
    readTerminals(terminals);

    NiceTreeNode::adjNodes = adjNodes;
    NiceTreeNode* root = NiceTreeNode::readInput(numNodes);
    // root -> prettyPrintNiceTree();
    return 0;
}
