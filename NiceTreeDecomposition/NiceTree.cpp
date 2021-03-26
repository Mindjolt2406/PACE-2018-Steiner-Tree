#include<bits/stdc++.h>
#include "NiceTree.hpp"

// Change to consider adding edges as well
NiceTreeNode::NiceTreeNode(vector<int> &bag) : nodeID(NiceTreeNode::classNodeID++) {
    this -> bag = bag;
    this -> parent = NULL;
}

void NiceTreeNode::generateNodesLeaf() {
    if(bag.size() == 0) {
        return;
    }

    this -> niceNodeType = NiceNodeType::VERTEX;
    vector<int> childBag = bag;
    childBag.pop_back();
    NiceTreeNode* childNode = new NiceTreeNode(childBag);
    children.push_back(childNode);
    childNode -> setParent(this);
    childNode -> generateNodesLeaf();
}


void NiceTreeNode::generateDiffNodes(vector<int> &introduceNodes, 
vector<int> &forgetNodes, NiceTreeNode* destNode) {
    set<int> currNodes(bag.begin(), bag.end());
    set<int> destNodes(destNode -> bag.begin(), destNode -> bag.end());

    for(auto node : currNodes) {
        if(!destNodes.count(node)) {
            introduceNodes.push_back(node);
        }
    }

    for(auto node : destNodes) {
        if(!currNodes.count(node)) {
            forgetNodes.push_back(node);
        }
    }
}

void NiceTreeNode::generateNodes(vector<NiceTreeNode*> &futureChildren) {
    if(futureChildren.size() == 1) {
        // Get introduce and forget nodes
        vector<int> introduceNodes, forgetNodes;
        generateDiffNodes(introduceNodes, forgetNodes, futureChildren.back());
        generateIFNodes(introduceNodes, forgetNodes, futureChildren.back());
    } else {
        // Generate join nodes
        this -> niceNodeType = NiceNodeType::JOIN;
        NiceTreeNode* leftJoinNode = new NiceTreeNode(this -> bag);
        NiceTreeNode* rightJoinNode = new NiceTreeNode(this -> bag);
        this -> children = {leftJoinNode, rightJoinNode};

        vector<NiceTreeNode*> leftFutureChildren, rightFutureChildren;
        leftFutureChildren.push_back(futureChildren.back());
        rightFutureChildren = futureChildren;
        rightFutureChildren.pop_back();
        leftJoinNode -> setParent(this);
        rightJoinNode -> setParent(this);
        leftJoinNode -> generateNodes(leftFutureChildren);
        rightJoinNode -> generateNodes(rightFutureChildren);
    }
}

void NiceTreeNode::generateIFNodes(vector<int> &introduceNodes, 
vector<int> &forgetNodes, NiceTreeNode* destNode) {

    // Introduce nodes are introduced by going from child -> parent and hence have one less vertex than the parent
    if(introduceNodes.size() + forgetNodes.size() == 1) {
        children.push_back(destNode);
        destNode -> setParent(this);
        this -> niceNodeType = (introduceNodes.size() == 1) ? NiceNodeType::VERTEX : NiceNodeType::FORGET;
    } else {
        if(!introduceNodes.empty()) {
            this -> niceNodeType = NiceNodeType::VERTEX;
            int introduceNode = introduceNodes.back();
            introduceNodes.pop_back();
            vector<int> childBag = bag;
            childBag.erase(remove(childBag.begin(), childBag.end(), introduceNode), childBag.end());
            NiceTreeNode* childNode = new NiceTreeNode(childBag);
            this -> children.push_back(childNode);
            childNode -> setParent(this);
            childNode -> generateIFNodes(introduceNodes, forgetNodes, destNode);
        } else if(!forgetNodes.empty()) {
            this -> niceNodeType = NiceNodeType::FORGET;
            int forgetNode = forgetNodes.back();
            forgetNodes.pop_back();
            vector<int> childBag = bag;
            childBag.push_back(forgetNode);
            NiceTreeNode* childNode = new NiceTreeNode(childBag);
            this -> children.push_back(childNode);
            childNode -> setParent(this);
            childNode -> generateIFNodes(introduceNodes, forgetNodes, destNode);
        }
    }
}

void NiceTreeNode::setNiceNodeType(NiceNodeType niceNodeType) {
    this -> niceNodeType = niceNodeType;
} 

// Reads the treewidth section of the input
void NiceTreeNode::readInput() {
    string sectionName, treeName, decompositionName, newLine;
    cin >> sectionName >> treeName >> decompositionName;
    string sName, tdName;
    cin >> sName >> tdName;
    int bagNum, treeWidth, nodesNum;
    cin >> bagNum >> treeWidth >> nodesNum;

    getline(cin, newLine);
    vector<NiceTreeNode*> niceTreeNodes;
    for(int i = 0; i < bagNum; i++) {
        string line;
        getline(cin, line);
        stringstream ss(line);
        string word;
        vector<int> bag;
        while (ss >> word) {
            if(word != "b") {
                bag.push_back(stoi(word));
            }
        }

        niceTreeNodes.push_back(new NiceTreeNode(bag));
    }

    vector<vector<int> > adjNodes(bagNum);
    for(int i = 0; i < bagNum - 1; i++) {
        int u, v;
        cin >> u >> v;
        u--; v--;
        adjNodes[u].push_back(v);
        adjNodes[v].push_back(u);
    }

    dfsNodes(0, -1, adjNodes, niceTreeNodes);

    // for(auto it : niceTreeNodes) dfsNiceTreeNodes(it);

    NiceTreeNode* finalRoot = niceTreeNodes.front() -> generateRoot();

    dfsNiceTreeNodes(finalRoot);
}

void NiceTreeNode::dfsNodes(int currNode, int parentNode, vector<vector<int> > &adj, vector<NiceTreeNode*> &niceTreeNodes) {
    bool isLeaf = true;
    vector<NiceTreeNode*> childTreeNodes;
    for(auto childNode : adj[currNode]) {
        if(childNode != parentNode) {
            childTreeNodes.push_back(niceTreeNodes[childNode]);
            isLeaf = false;
            dfsNodes(childNode, currNode, adj, niceTreeNodes);
        }
    }

    // t(currNode, childTreeNodes.size());
    if(isLeaf) {
        niceTreeNodes[currNode] -> generateNodesLeaf();
    } else {
        niceTreeNodes[currNode] -> generateNodes(childTreeNodes);
    }
}

void NiceTreeNode::dfsNiceTreeNodes(NiceTreeNode* niceTreeNode, int offsetNum) {
    string offset(offsetNum, '-');
    if(offsetNum) offset += "  ";
    cerr << offset; t(niceTreeNode -> nodeID);
    if(niceTreeNode -> parent != NULL) {cerr << offset; t(niceTreeNode -> parent -> nodeID);}
    cerr << offset; cerr << "niceNodeType: " << niceNodeTypeArr[(int)niceTreeNode -> niceNodeType] << endl;
    cerr << offset; t(niceTreeNode -> children);
    cerr << offset; t(niceTreeNode -> bag);
    cerr << endl;
    
    for(auto it : niceTreeNode -> children) {
        dfsNiceTreeNodes(it, offsetNum+2);
    }
}

NiceTreeNode* NiceTreeNode::generateRoot() {

    // Non-legitimate call
    if(this -> parent != NULL) {
        return this;
    }

    // bag is empty, don't generate more parent nodes
    if(this -> bag.size() == 0) {
        return this;
    }

    vector<int> parentBag = this -> bag;
    parentBag.pop_back();
    NiceTreeNode* parentNode = new NiceTreeNode(parentBag);
    parentNode -> children.push_back(this);
    parentNode -> setNiceNodeType(NiceNodeType::FORGET);

    return parentNode -> generateRoot();
}

// Add a node "introducing" the edge just before we forget that vertex. 
void NiceTreeNode::addIntroduceEdgeNodeForVertex(vector<pair<int, int> > &edgesToAdd) {
    int parentChildIndex = 0;
    if(this -> parent -> niceNodeType == NiceNodeType::FORGET) {
        parentChildIndex = (parent -> children.front() -> nodeID == this -> nodeID) ? 0 : 1;
    }

    for(auto edge : edgesToAdd) {
        NiceTreeNode* newEdgeNode = new NiceTreeNode(this -> bag);

        // Handling parent and child pointers for the new node
        this -> parent -> children[parentChildIndex] = newEdgeNode;
        newEdgeNode -> parent = this -> parent;
        newEdgeNode -> children.push_back(this);
        this -> parent = newEdgeNode;
        newEdgeNode -> niceNodeType = NiceNodeType::EDGE;

        newEdgeNode -> edgeList = this -> edgeList;
        newEdgeNode -> edgeList.push_back(edge);
    }
}

void static NiceTreeNode::addIntroduceEdgeNodes(int numNodes) {
    for(int i = 0; i < numNodes; i++) {
        vector<pair<int, int> > edgesToAdd;
        int currDepthHighestNodeVertex = highestNodeVertex[i] -> depthNode;
        for(auto child : adjNodes[i]) {
            if(highestNodeVertex[child] -> depthNode < currDepthHigestNodeVertex) {
                edgesToAdd.push_back(make_pair(i, child));
            }
        }

        highestNodeVertex[i] -> addIntroduceEdgeNodeForVertex(edgesToAdd);
    }
}

void static NiceTreeNode::calculateDepthNodes(NiceTreeNode* currNode, int depth = 0) {
    currNode -> depthNode = depth;

    if(currNode -> niceNodeType == NiceNodeType :: FORGET) {
        highestNodeVertex[currNode -> vertex] = currNode -> children.front();
    }

    for(auto childNode : currNode -> children) {
        calculateDepthNodes(childNode, depth + 1);
    }
}

void NiceTreeNode::setParent(NiceTreeNode* parentNode) {
    this -> parent = parentNode;
}

ostream& operator <<(ostream &os, NiceTreeNode* niceTreeNode) {
    os << (int)niceTreeNode -> nodeID;
    return os;
}

int NiceTreeNode::classNodeID = 0;
vector<string> NiceTreeNode::niceNodeTypeArr = {"Introduce", "Edge", "Forget", "Join", "None"};
//
/*
SECTION Tree Decomposition
s td 4 2 5
b 1 2
b 1 2 3
b 1 3 4
b 1 4 5
1 2
2 3
3 4

SECTION Tree Decomposition
s td 4 2 7
b 1 2 5
b 3 4 5
b 1 5 6
b 1 7
1 2
1 3
1 4

*/
