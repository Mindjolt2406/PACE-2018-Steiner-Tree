#include "Recurrence/Recurrence.hpp"
#include "Recurrence/Partition.hpp"
#include "NiceTreeDecomposition/NiceTree.hpp"
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

void readTerminals(set<int> &terminals) {
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

        terminals.insert(u);
    }

    cin >> endName;
}

int main() {
    __;
    int numNodes;
    vector<vector<int> > adjNodes;
    map<pair<int, int>, int> edgeWeight;
    set<int> terminals;

    readGraph(numNodes, adjNodes, edgeWeight);

    readTerminals(terminals);

    NiceTreeNode::adjNodes = adjNodes;
    NiceTreeNode* root = NiceTreeNode::readInput(numNodes);

    root -> prettyPrintNiceTree();

    // Partition::precomputeMaps(5);
    // Max bag size since we add one terminal node
    Partition::precomputeMaps(NiceTreeNode::treeWidth+2); 

    Recurrence::adjNodes = adjNodes;
    Recurrence::edgeWeight = edgeWeight;
    Recurrence::terminals = terminals;
    Recurrence::addOneTerminalAllBags(root);
    // t(root -> bag);
    t(Recurrence::specialTerminal, Recurrence::terminals);
    cout << Recurrence::calculateDP(root) << endl;
    return 0;
}

/*
SECTION GRAPH
NODES 5
EDGES 5
E 1 2 1
E 2 3 1
E 3 4 1
E 4 5 1
E 5 1 1
END 

SECTION Terminals
Terminals 2
T 1
T 4
END

SECTION Tree Decomposition
s td 4 2 5
b 3 1 2
b 2 1 2 3
b 1 1 3 4
b 4 1 4 5
3 2
2 1
4 1

*/