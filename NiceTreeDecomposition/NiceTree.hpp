#ifndef __NICE__TREE_
#define __NICE__TREE_

#include<bits/stdc++.h>
#define INF 1e9+1
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

// Change to consider adding edges as well
class NiceTreeNode {
    public: 
    static int classNodeID;
    static vector<string> niceNodeTypeArr;
    
    vector<NiceTreeNode*> children;
    vector<int> bag;
    NiceNodeType niceNodeType = NiceNodeType::NONE;
    int nodeID;

    NiceTreeNode(vector<int> &bag);

    void generateNodesLeaf();


    void generateDiffNodes(vector<int> &introduceNodes, 
    vector<int> &forgetNodes, NiceTreeNode* destNode);

    void generateNodes(vector<NiceTreeNode*> &futureChildren);

    void generateIFNodes(vector<int> &introduceNodes, 
    vector<int> &forgetNodes, NiceTreeNode* destNode);

    void setNiceNodeType(NiceNodeType niceNodeType) ;

    void static readInput();

    void static dfsNodes(int currNode, int parentNode, vector<vector<int> > &adj, vector<NiceTreeNode*> &niceTreeNodes);

    void static dfsNiceTreeNodes(NiceTreeNode* niceTreeNode, int offsetNum = 0);

}; 

#endif