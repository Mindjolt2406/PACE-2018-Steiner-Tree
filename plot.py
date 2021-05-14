from matplotlib import pyplot as plt

timeTaken = {6 : 427, 7 : 540, 8 : 925, 9 : 879, 10 : 1200, 11 : 2280, 12 : 840, 13: 1525, 14 : 1853}
nodes = {6 : 232, 7 : 216, 8 : 226, 9 : 229, 10 : 230, 11 : 244, 12 : 572, 13 : 1906, 14 : 3398}
edges = {6 : 256, 7 : 288, 8 : 306, 9 : 312, 10 : 313, 11 : 337, 12 : 662, 13 : 2083, 14 : 3966}
bag = {6 : 226, 7 : 225,8 : 222,9 : 224,10 : 225,11 : 231,12 : 565,13 : 1896,14 : 3397}
join = {6 : 67, 7 : 74, 8 : 66, 9 : 70, 10 : 71, 11 : 73, 12 : 158, 13 : 444, 14 : 999}
joinBag = {6 : 212, 7 : 290, 8 : 266, 9 : 289, 10 : 295, 11 : 325, 12 : 538, 13 : 1380, 14 : 3224}
joinBagPow = {6 : 470714, 7 : 334696, 8 : 576990, 9 : 562715, 10 : 731563, 11 : 1323799, 12 : 511502, 13 : 951118, 14 : 1307662}

l = []
for i in range(6, 15):
    temp = [timeTaken[i], nodes[i], edges[i], bag[i], joinBagPow[i]]
    l.append(temp)


nodes = [i[1] for i in l]
timer = [i[0] for i in l]

plt.xlabel("# Nodes")
plt.ylabel("Time")
plt.plot(nodes, timer, 'o')
plt.show()

edges = [i[2] for i in l]

plt.xlabel("# Edges")
plt.ylabel("Time")
plt.plot(edges, timer, 'o')
plt.show()

nodesEdges = [i[2] * i[1] for i in l]
plt.xlabel("Nodes x Edges")
plt.ylabel("Time")
plt.plot(nodesEdges, timer, 'o')
plt.show()

nodesEdgesBag= [i[2] * i[1] * i[3] for i in l]
plt.xlabel("Nodes x Edges x Bags")
plt.ylabel("Time")
plt.plot(nodesEdgesBag,timer, 'o')
plt.show()

nodesJoin = [i[4] for i in l]
plt.xlabel("Join Nodes")
plt.ylabel("Time")
plt.plot(nodesJoin, timer, 'o')
plt.show()

nodesJoin = [i[4] for i in l]
plt.xlabel("Join Nodes")
plt.ylabel("Num nodes")
plt.plot(nodesJoin, [i[1] for i in l])
plt.show()