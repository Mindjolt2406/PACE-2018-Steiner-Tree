#ifndef __NICE__TREE_
#define __NICE__TREE_

#include<bits/stdc++.h>
#define INF 1e18+1
// g++ -std=c++17 -Wl,-stack_size -Wl,0x10000000 main.cpp
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
// mt19937 rng(chrono::steady_clock::now().time_since_epoch().count());
template <int> ostream& operator<<(ostream& os, const vector<int>& v) { os << "["; for (int i = 0; i < v.size(); ++i) { if(v[i]!=INF) os << v[i]; else os << "INF";if (i != v.size() - 1) os << ", "; } os << "]"; return os; } 
template <typename T> ostream& operator<<(ostream& os, const vector<T>& v) { os << "["; for (int i = 0; i < v.size(); ++i) { os << v[i]; ;if (i != v.size() - 1) os << ", "; } os << "]"; return os; } 
template <typename T> ostream& operator<<(ostream& os, const set<T>& s) {os << "{"; for(auto it : s) {if(it != *s.rbegin()) os << it << ", "; else os << it;} os << "}"; return os;}
template<class A, class B> ostream& operator<<(ostream& out, const pair<A, B> &a){ return out<<"("<<a.first<<", "<<a.second<<")";}

enum class NiceNodeType {
    NONE, // Null nodes
    EDGE, 
    FORGET, 
    JOIN, 
    VERTEX, 
    WEIRD
}; 

class NiceTreeNode {
    public: 
    static int classNodeID;
    static vector<string> niceNodeTypeArr;
    static vector< vector<int> >  adjNodes;
    static map<int, NiceTreeNode* > highestNodeVertex; 
    static int treeWidth;

    const int nodeID;
    NiceTreeNode* parent;
    vector<NiceTreeNode*> children;
    vector<int> bag;
    NiceNodeType niceNodeType = NiceNodeType::NONE;

    // redundant, only used for introducing edges and comparing depths of vertex ancestors
    int depthNode; 

    // Edge introduced
    pair<int, int> edge;

    // Vertex introduced or forgotten
    int vertex; 

    NiceTreeNode(vector<int> &bag);

    void generateNodesLeaf();
    
    // Called only for root with a non zero bag size
    NiceTreeNode* generateRoot();

    void generateDiffNodes(vector<int> &introduceNodes, 
    vector<int> &forgetNodes, NiceTreeNode* destNode);

    void generateNodes(vector<NiceTreeNode*> &futureChildren);

    void generateIFNodes(vector<int> &introduceNodes, 
    vector<int> &forgetNodes, NiceTreeNode* destNode);

    // Setters
    void setNiceNodeType(NiceNodeType niceNodeType) ;

    void setParent(NiceTreeNode* parent);

    void addIntroduceEdgeNodeForVertex(vector<pair<int, int> > &edgesToAdd);

    void prettyPrintNiceTree(int offsetNum = 0);

    bool isLeaf();

    static NiceTreeNode* readInput(int numNodes);

    static void dfsNodes(int currNode, int parentNode, vector<vector<int> > &adj, vector<NiceTreeNode*> &niceTreeNodes);

    static void calculateDepthNodes(NiceTreeNode* currNode, int depth = 0);

    static void addIntroduceEdgeNodes(int numNodes);

    friend ostream& operator<<(ostream &os, NiceNodeType &niceNodeType);
}; 

#endif