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
template<class A, class B> ostream& operator<<(ostream& out, const pair<A, B> &a){ return out<<"("<<a.first<<", "<<a.second<<")";}
template <int> ostream& operator<<(ostream& os, const vector<int>& v) { os << "["; for (int i = 0; i < v.size(); ++i) { if(v[i]!=INF) os << v[i]; else os << "INF";if (i != v.size() - 1) os << ", "; } os << "]"; return os; } 
template <typename T> ostream& operator<<(ostream& os, const vector<T>& v) { os << "["; for (int i = 0; i < v.size(); ++i) { os << v[i]; ;if (i != v.size() - 1) os << ", "; } os << "]"; return os; } 

enum class NiceNodeType {
    VERTEX, 
    EDGE, 
    FORGET, 
    JOIN, 
    NONE // Null nodes
}; 

vector<string> niceNodeTypeArr = {"Introduce", "Edge", "Forget", "Join", "None"};

// Change to consider adding edges as well
class NiceTreeNode {
    public: 
    static int classNodeID;
    vector<NiceTreeNode*> children;
    vector<int> bag;
    NiceNodeType niceNodeType = NiceNodeType::NONE;
    int nodeID;

    NiceTreeNode(vector<int> &bag) : nodeID(NiceTreeNode::classNodeID++) {
        this -> bag = bag;
    }

    NiceTreeNode(vector<int> &bag, NiceNodeType niceNodeType) : nodeID(NiceTreeNode::classNodeID++) {
        this -> bag = bag;
        this -> niceNodeType = niceNodeType;
    }

    NiceTreeNode(vector<int> &bag, vector<NiceTreeNode*> &children, NiceNodeType niceNodeType) : nodeID(NiceTreeNode::classNodeID++) {
        this -> bag = bag;
        this -> niceNodeType = niceNodeType;
        this -> children = children;
    }

    void generateNodesLeaf() {
        if(bag.size() == 0) {
            return;
        }

        this -> niceNodeType = NiceNodeType::VERTEX;
        vector<int> childBag = bag;
        childBag.pop_back();
        NiceTreeNode* childNode = new NiceTreeNode(childBag);
        children.pu(childNode);
        childNode -> generateNodesLeaf();
    }


    void generateDiffNodes(vector<int> &introduceNodes, 
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

    void generateNodes(vector<NiceTreeNode*> &futureChildren) {
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
            leftJoinNode -> generateNodes(leftFutureChildren);
            rightJoinNode -> generateNodes(rightFutureChildren);
        }
    }

    void generateIFNodes(vector<int> &introduceNodes, 
    vector<int> &forgetNodes, NiceTreeNode* destNode) {

        // Introduce nodes are introduced by going from child -> parent and hence have one less vertex than the parent
        t(this->nodeID, this->bag, introduceNodes, forgetNodes);
        if(introduceNodes.size() + forgetNodes.size() == 1) {
            children.pu(destNode);
            this -> niceNodeType = (introduceNodes.size() == 1) ? NiceNodeType::VERTEX : NiceNodeType::FORGET;
        } else {
            if(!introduceNodes.empty()) {
                this -> niceNodeType = NiceNodeType::VERTEX;
                int introduceNode = introduceNodes.back();
                introduceNodes.pop_back();
                vector<int> childBag = bag;
                t(introduceNode, childBag);
                childBag.erase(remove(childBag.begin(), childBag.end(), introduceNode), childBag.end());
                t(childBag);
                NiceTreeNode* childNode = new NiceTreeNode(childBag);
                this -> children.push_back(childNode);
                childNode -> generateIFNodes(introduceNodes, forgetNodes, destNode);
            } else if(!forgetNodes.empty()) {
                this -> niceNodeType = NiceNodeType::FORGET;
                int forgetNode = forgetNodes.back();
                forgetNodes.pop_back();
                vector<int> childBag = bag;
                childBag.push_back(forgetNode);
                NiceTreeNode* childNode = new NiceTreeNode(childBag, NiceNodeType::FORGET);
                this -> children.push_back(childNode);
                childNode -> generateIFNodes(introduceNodes, forgetNodes, destNode);
            }
        }
    }

    void setNiceNodeType(NiceNodeType niceNodeType) {
        this -> niceNodeType = niceNodeType;
    } 

    void static readInput() {
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
        dfsNiceTreeNodes(niceTreeNodes.front());
    }

    void static dfsNodes(int currNode, int parentNode, vector<vector<int> > &adj, vector<NiceTreeNode*> &niceTreeNodes) {
        bool isLeaf = true;
        vector<NiceTreeNode*> childTreeNodes;
        for(auto childNode : adj[currNode]) {
            if(childNode != parentNode) {
                childTreeNodes.push_back(niceTreeNodes[childNode]);
                isLeaf = false;
                dfsNodes(childNode, currNode, adj, niceTreeNodes);
            }
        }

        t(currNode, childTreeNodes.size());
        if(isLeaf) {
            niceTreeNodes[currNode] -> generateNodesLeaf();
        } else {
            niceTreeNodes[currNode] -> generateNodes(childTreeNodes);
        }
    }

    void static dfsNiceTreeNodes(NiceTreeNode* niceTreeNode, int offsetNum = 0) {
        string offset(offsetNum, '-');
        offset += "  ";
        cerr << offset; t(niceTreeNode -> nodeID);
        cerr << offset; cerr << "niceNodeType: " << niceNodeTypeArr[(int)niceTreeNode -> niceNodeType] << endl;
        cerr << offset; t(niceTreeNode -> children);
        cerr << offset; t(niceTreeNode -> bag);
        cerr << endl;
        
        for(auto it : niceTreeNode -> children) {
            dfsNiceTreeNodes(it, offsetNum+2);
        }
    }

}; 

 ostream& operator <<(ostream &os, NiceTreeNode* niceTreeNode) {
    os << (int)niceTreeNode -> nodeID;
    return os;
}

int NiceTreeNode::classNodeID = 0;

int main() {
    __;
    // NiceTreeNode::classNodeID = 0;
    NiceTreeNode::readInput();
    return 0;
}

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
