#include<bits/stdc++.h>
#include "Recurrence.hpp"
#include "Partition.hpp"

void Recurrence::addOneTerminalAllBags(NiceTreeNode* root) {
    specialTerminal = *terminals.begin();
    if(!count(root -> bag.begin(), root -> bag.end(), specialTerminal)) {
        root -> bag.push_back(specialTerminal);
    }

    for(auto childNode : root -> children) {
        addOneTerminalAllBags(childNode);
    }
}

ll Recurrence::computeDP(NiceTreeNode* root, set<int> forestSet, set<set<int> > partition) {
    cerr << "--------" << endl;
    cerr << "root: " << *root << endl;
    t(forestSet, partition);
    // Corresponding DP Key - {t, X, P}
    auto dpKey = make_pair(root -> nodeID, make_pair(forestSet, partition));

    // Check if node is already computed and return if so
    if(dp.count(dpKey)) {
        t("computed", dpKey, dp[dpKey]);
        return dp[dpKey];
    }

    // Base Condition - Leaf Node
    if(root -> niceNodeType == NiceNodeType::NONE) {
        dp[dpKey] = computeLeaf(root, forestSet, partition);
        t(dpKey, dp[dpKey]);
        return dp[dpKey];
    } 

    // Check the 4 kinds of nodes and call their appropriate functions

    // Introduce Vertex
    if(root -> niceNodeType == NiceNodeType::VERTEX) {
        dp[dpKey] = computeIntroduceVertex(root, forestSet, partition);
        t(dpKey, dp[dpKey]);
        return dp[dpKey];
    }


    // Introduce Edge
    if(root -> niceNodeType == NiceNodeType::EDGE) {
        dp[dpKey] = computeIntroduceEdge(root, forestSet, partition);
        t(dpKey, dp[dpKey]);
        return dp[dpKey];
    }

    // Forget Node
    if(root -> niceNodeType == NiceNodeType::FORGET) {
        dp[dpKey] = computeForgetVertex(root, forestSet, partition);
        t(dpKey, dp[dpKey]);
        return dp[dpKey];
    }

    // Join Node
    if(root -> niceNodeType == NiceNodeType::JOIN) {
        dp[dpKey] = computeJoinNode(root, forestSet, partition);
        t(dpKey, dp[dpKey]);
        return dp[dpKey];
    } else { // Weird Node
        dp[dpKey] = computeWeirdNode(root, forestSet, partition);
        t(dpKey, dp[dpKey]);
        return dp[dpKey];
    }
}

ll Recurrence::computeLeaf(NiceTreeNode* root, set<int> &forestSet, set<set<int> > &partition) {
    // Corresponding DP Key - {t, X, P}
    auto dpKey = make_pair(root -> nodeID, make_pair(forestSet, partition));

    dp[dpKey] = INF+1;

    // Either the forest set and the partition is empty, or the forest contains the terminal and the partition has only one terminal. 

    // Explicitly check these two conditions, otherwise raise INF 
    // Only terminal
    t(forestSet, partition);
    if(forestSet.size() == 1 && partition.size() == 1 && (*partition.begin() == forestSet)  && *forestSet.begin() == specialTerminal)  {
        return dp[dpKey] = 0;
    }

    // Technically, they shouldn't arise, but who knows? 

    return dp[dpKey] = INF;
}

ll Recurrence::computeIntroduceVertex(NiceTreeNode* root, set<int> forestSet, set<set<int> > partition) {
    // Corresponding DP Key - {t, X, P}
    auto dpKey = make_pair(root -> nodeID, make_pair(forestSet, partition));

    dp[dpKey] = INF+1;

    // Vertex we are introducing
    int vertex = root -> vertex;

    // Child node
    NiceTreeNode* childRoot = root -> children.front();

    t(root -> nodeID, childRoot -> nodeID, vertex, dpKey);

    // Check if the vertex is a terminal and if it's in the set or not

    // This shouldn't happen as we aren't counting a terminal
    if(terminals.count(vertex) && !forestSet.count(vertex)) { 
        return dp[dpKey] = INF;
    }

    if(forestSet.count(vertex) && vertex != specialTerminal) { 
        // Remove the vertex from it's set and try
        set<int> tempAloneSet;
        tempAloneSet.insert(vertex);
        if(partition.count(tempAloneSet)) {
            partition.erase(tempAloneSet);
            forestSet.erase(vertex);
            t("Introduce", childRoot -> nodeID, forestSet, partition);
            return dp[dpKey] = min(dp[dpKey], computeDP(childRoot, forestSet, partition));
        } else { // Vertex not isolated
            return dp[dpKey] = INF;
        }
    } else { // Just recurse
        return dp[dpKey] = computeDP(childRoot, forestSet, partition);
    }
}

ll Recurrence::computeForgetVertex(NiceTreeNode* root, set<int> forestSet, set<set<int> > partition) {
     // Corresponding DP Key - {t, X, P}
    auto dpKey = make_pair(root -> nodeID, make_pair(forestSet, partition));
    // t(dpKey);

    dp[dpKey] = INF + 1;

    // Vertex we are forgetting a.k.a introducing to the child node
    int vertex = root -> vertex;

    // Child node
    NiceTreeNode* childRoot = root -> children.front();

    // We are using the vertex in the solution, build the corresponding partitions
    if(vertex != specialTerminal) {
        set<set<int> > tempPartition = partition;        
        set<int> tempForestSet = forestSet;

        // Since we're taking the vertex, insert it into the solution
        tempForestSet.insert(vertex);

        vector<set<int> > blocks(tempPartition.begin(), tempPartition.end());

        // For each partition, add the vertex to that set
        // Remove the set, insert the element and add it back
        for(auto block : blocks) {
            tempPartition.erase(block);
            block.insert(vertex);
            tempPartition.insert(block);
            t(vertex, tempForestSet, tempPartition);
            dp[dpKey] = min(computeDP(childRoot, tempForestSet, tempPartition), dp[dpKey]);
            tempPartition.erase(block);
            block.erase(vertex);
            tempPartition.insert(block);
        }

        // Also add an independent set {vertex} to partition
        // Cannot do this
        // set<int> tempAloneSet;
        // tempAloneSet.insert(vertex);
        // tempPartition.insert(tempAloneSet);

        // dp[dpKey] = min(computeDP(childRoot, tempForestSet, tempPartition), dp[dpKey]);
    }
    
    // We are not using the vertex in the solution, just recurse
    // We also just recurse if we are forgetting the special terminal
    return dp[dpKey] = min(computeDP(childRoot, forestSet, partition), dp[dpKey]);
}

ll Recurrence::computeIntroduceEdge(NiceTreeNode* root, set<int> &forestSet, set<set<int> > &partition) {
     // Corresponding DP Key - {t, X, P}
    auto dpKey = make_pair(root -> nodeID, make_pair(forestSet, partition));

    dp[dpKey] = INF + 1;

    pair<int, int> edge = root -> edge;

    // Child node
    NiceTreeNode* childRoot = root -> children.front();

    // If u or v is not in the solution, no need to add the edge, just recurse
    if(!forestSet.count(edge.first) || !forestSet.count(edge.second)) {
        return dp[dpKey] = computeDP(childRoot, forestSet, partition);
    }

    set<int> tempBlock;
    // Check if u and v are not in the same block, don't add the edge then, just recurse
    if(!edgeInSameBlock(edge, partition, tempBlock)) {
        return dp[dpKey] = computeDP(childRoot, forestSet, partition);
    } else {
        // Don't take the edge (u, v)
        dp[dpKey] = min(dp[dpKey], computeDP(childRoot, forestSet, partition));
        t(dp[dpKey], "Edgeeee");

        // u and v are in the same block stored as tempBlock
        // Delete tempBlock, make partitions by iterating over 2^|tempBlock| - 2 (very imp)
        // Add each pair of partitions to the partition and recurse

        // Delete tempBlock
        partition.erase(tempBlock);

        // Erase u and v as we are separating them
        tempBlock.erase(edge.first);
        tempBlock.erase(edge.second);

        int tempBlockSize = tempBlock.size();
        vector<int> tempBlockList(tempBlock.begin(), tempBlock.end());

        if(tempBlockSize == 0) { // Only u and v existed, then just recruse on that
            set<int> blockA, blockB;
            blockA.insert(edge.first);
            blockB.insert(edge.second);
            partition.insert(blockA);
            partition.insert(blockB);
            ll tempAns = computeDP(childRoot, forestSet, partition);
            dp[dpKey] = min(dp[dpKey], tempAns + edgeWeight[edge]);

            t(dp[dpKey], dpKey);
            return dp[dpKey];
        }

        for(int mask = 0; mask < (1 << (tempBlockSize)); mask++) {
            // if(mask == 0 || mask == (1 << tempBlockSize)-1) { // The other partition is empty
            //     continue;
            // }

            set<int> blockA, blockB;
            blockA.insert(edge.first);
            blockB.insert(edge.second);
            buildSetForEdgePartition(tempBlockList, blockA, blockB, mask);

            // Build a temporary partition
            set<set<int> > tempPartition = partition;

            // Add the two newly divided blocks back
            tempPartition.insert(blockA);
            tempPartition.insert(blockB);

            // Recurse and add weight of edge
            t(edge, edgeWeight[edge], tempPartition);
            dp[dpKey] = min(dp[dpKey], computeDP(childRoot, forestSet, tempPartition) + edgeWeight[edge]);
        }

        t(dp[dpKey], dpKey);
        return dp[dpKey];
    }
}

ll Recurrence::computeJoinNode(NiceTreeNode* root, set<int> &forestSet, set<set<int> > &partition) {
     // Corresponding DP Key - {t, X, P}
    auto dpKey = make_pair(root -> nodeID, make_pair(forestSet, partition));

    dp[dpKey] = INF + 1;

    NiceTreeNode* childLeft = root -> children.front();
    NiceTreeNode* childRight = root -> children.back();

    // Get all compatible P1s and P2s such that their merge is acyclic and they form P
    vector<pair<vector<set<int> >,vector< set<int> > > > pairPartitions = Partition::permutationMapper(partition);

    for(auto pairPart : pairPartitions) {
        set<set<int> > finalPartLeft(pairPart.first.begin(), pairPart.first.end());
        set<set<int> > finalPartRight(pairPart.second.begin(), pairPart.second.end());
        t("JOIN_IN", partition, finalPartLeft, finalPartRight);

        dp[dpKey] = min(computeDP(childLeft, forestSet, finalPartLeft) + computeDP(childRight, forestSet, finalPartRight), dp[dpKey]);

        // DO IT THE OTHER WAY TOOO!
        dp[dpKey] = min(computeDP(childLeft, forestSet, finalPartRight) + computeDP(childRight, forestSet, finalPartLeft), dp[dpKey]);
    }

    return dp[dpKey];
}

ll Recurrence::computeWeirdNode(NiceTreeNode* root, set<int> &forestSet, set<set<int> > &partition) {
    auto dpKey = make_pair(root -> nodeID, make_pair(forestSet, partition));

    dp[dpKey] = INF + 1;

    // Child node
    NiceTreeNode* childRoot = root -> children.front();

    return dp[dpKey] = computeDP(childRoot, forestSet, partition);
}

ll Recurrence::calculateDP(NiceTreeNode* root) {
    set<int> forestSet;
    set<set<int> > partition;
    forestSet.insert(root -> bag.front());
    partition.insert(forestSet);
    return computeDP(root, forestSet, partition);
}

bool Recurrence::removeVertexFromPartition(int vertex, set<set<int> > &partition) {
    set<int> eraseSet;
    for(auto childSet : partition) { 
        if(childSet.count(vertex)) {
            eraseSet = childSet;
        }
    }

    t(vertex, partition);
    if(!eraseSet.empty()) {
        partition.erase(eraseSet);
        eraseSet.erase(vertex);
        if(!eraseSet.empty()) {
            partition.insert(eraseSet);
        }
        t(partition);
        return true;
    }

    return false;
}

bool Recurrence::edgeInSameBlock(pair<int, int> edge, set<set<int> > &partition, set<int> &tempBlock) {
    for(auto childSet : partition) {
        if(childSet.count(edge.first) && childSet.count(edge.second)) {
            tempBlock = childSet;
            return true;
        }
    }

    return false;
}

void Recurrence::buildSetForEdgePartition(vector<int> &tempBlockList, set<int> &blockA, set<int> &blockB, int mask) {
    for(int i = 0; i < tempBlockList.size(); i++) {
        if(mask&1) {
            blockA.insert(tempBlockList[i]);
        } else {
            blockB.insert(tempBlockList[i]);
        }

        mask >>= 1;
    }
}


map<pair<int, pair<set<int>, set<set<int> > > > , ll> Recurrence::dp; 
vector<vector<int> > Recurrence::adjNodes;
map<pair<int, int>, int> Recurrence::edgeWeight;
set<int> Recurrence::terminals;
int Recurrence::specialTerminal;