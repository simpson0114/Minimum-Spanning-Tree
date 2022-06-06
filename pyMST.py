import sys
#    Python 3 program for
#    Prim's algorithm using adjacency list
class Edge :
    #  Source node
    #  Destination node
    #  Edge weight
    def __init__(self, u, v, weight) :
        self.u = u
        self.v = v
        self.weight = weight
    

class HeapNode :
    def __init__(self) :
        self.vertex = 0
        self.key = 0
    

class ResultSet :
    def __init__(self) :
        self.parent = 0
        self.weight = 0
    

class MinHeap :
    def __init__(self, capacity) :
        self.capacity = capacity
        self.node = [None] * (capacity + 1)
        self.indexes = [0] * (capacity)
        self.node[0] = HeapNode()
        self.node[0].key = -sys.maxsize
        self.node[0].vertex = -1
        self.currentSize = 0
    
    def swap(self, a, b) :
        temp = self.node[a]
        self.node[a] = self.node[b]
        self.node[b] = temp
    
    def isEmpty(self) :
        return self.currentSize == 0
    
    def heapSize(self) :
        return self.currentSize
    
    def bubbleUp(self, pos) :
        parentIdx = int(pos / 2)
        currentIdx = pos
        while (currentIdx > 0 and 
               self.node[parentIdx].key > self.node[currentIdx].key) :
            currentNode = self.node[currentIdx]
            parentNode = self.node[parentIdx]
            self.indexes[currentNode.vertex] = parentIdx
            self.indexes[parentNode.vertex] = currentIdx
            self.swap(currentIdx, parentIdx)
            currentIdx = parentIdx
            parentIdx = int(parentIdx / 2)
        
    
    def insert(self, x) :
        self.currentSize += 1
        idx = self.currentSize
        self.node[idx] = x
        self.indexes[x.vertex] = idx
        self.bubbleUp(idx)
    
    def extractMin(self) :
        min = self.node[1]
        lastNode = self.node[self.currentSize]
        self.indexes[lastNode.vertex] = 1
        self.node[1] = lastNode
        self.node[self.currentSize] = None
        self.sinkDown(1)
        self.currentSize -= 1
        return min
    
    def sinkDown(self, k) :
        smallest = k
        #  left child
        leftChild = 2 * k
        #  right child
        rightChild = 2 * k + 1
        if (leftChild < self.heapSize() and 
            self.node[smallest].key > self.node[leftChild].key) :
            smallest = leftChild
        
        if (rightChild < self.heapSize() and 
            self.node[smallest].key > self.node[rightChild].key) :
            smallest = rightChild
        
        if (smallest != k) :
            smallestNode = self.node[smallest]
            kNode = self.node[k]
            self.indexes[smallestNode.vertex] = k
            self.indexes[kNode.vertex] = smallest
            self.swap(k, smallest)
            self.sinkDown(smallest)
        
class DSU :
    def __init__(self, n) :
        self.parent = []
        self.rank = []

        for _ in range(n):
            self.parent.append(-1)
            self.rank.append(1)
    
    def find(self, i) :
        if self.parent[i] == -1:
            return i
        self.parent[i] = self.find(self.parent[i])
        return self.parent[i]

    def unite(self, x, y) :
        s1 = self.find(x)
        s2 = self.find(y)

        if s1 != s2:
            if self.rank[s1] < self.rank[s2]:
                self.parent[s1] = s2
                self.rank[s2] += self.rank[s1]
            else:
                self.parent[s2] = s1
                self.rank[s1] += self.rank[s2]


class Graph :
    def __init__(self, vertices) :
        self.vertices = vertices
        self.adjacencylist = []
        self.edgelist = []
        i = 0
        while (i < self.vertices) :
            self.adjacencylist.append([])
            i += 1
        
    
    def addEdge(self, u, v, weight) :
        self.edgelist.append((weight, u, v))
        self.adjacencylist[u].append(Edge(u, v, weight))
        if (u == v) :
            #  self loop
            return
        
        self.adjacencylist[v].append(Edge(v, u, weight))
    
    def decreaseKey(self, minHeap, newKey, vertex) :
        index = minHeap.indexes[vertex]
        node = minHeap.node[index]
        node.key = newKey
        minHeap.bubbleUp(index)
    
    def primMST(self) :
        inHeap = [False] * (self.vertices)
        resultSet = [None] * (self.vertices)
        key = [0] * (self.vertices)
        heapNodes = [None] * (self.vertices)
        i = 0
        #  Set default value of heap nodes resultset and keys
        while (i < self.vertices) :
            heapNodes[i] = HeapNode()
            heapNodes[i].vertex = i
            heapNodes[i].key = sys.maxsize
            resultSet[i] = ResultSet()
            resultSet[i].parent = -1
            inHeap[i] = True
            key[i] = sys.maxsize
            i += 1
        
        heapNodes[0].key = 0
        minHeap = MinHeap(self.vertices)
        j = 0
        while (j < self.vertices) :
            minHeap.insert(heapNodes[j])
            j += 1
        
        i = 0
        while (minHeap.isEmpty() == False) :
            extractedNode = minHeap.extractMin()
            extractedVertex = extractedNode.vertex
            inHeap[extractedVertex] = False
            while (i < len(self.adjacencylist[extractedVertex])) :
                edge = self.adjacencylist[extractedVertex][i]
                if (inHeap[edge.v]) :
                    v = edge.v
                    w = edge.weight
                    if (key[v] > w) :
                        key[v] = w
                        self.decreaseKey(minHeap, w, v)
                        resultSet[v].parent = extractedVertex
                        resultSet[v].weight = w
                    
                
                i += 1
            
            i = 0
        
        result = 0
        print("\n\n Minimum Spanning Tree ")
        node = 1
        while (node < self.vertices) :
            print(" Edge (", resultSet[node].parent ,"-", node ,
                  ")  weight : ", resultSet[node].weight)
            result += resultSet[node].weight
            node += 1
        
        #  Display calculated result
        print(" Total minimum weight : ", result)
        return result

    def kruskalsMST(self):
        self.edgelist.sort()
    
        s = DSU(self.vertices)
        min_cost = 0
        print("\n\n Minimum Spanning Tree (Kruskal's) \n")
        for edge in self.edgelist:
            w = edge[0]
            x = edge[1]
            y = edge[2]

            # take that edge in MST if it does form a cycle
            if s.find(x) != s.find(y):
                s.unite(x, y)
                min_cost += w
                print("Edge (" + str(x) +  "-" + str(y) + ")  weight : " + str(w))
    
        print("Minimum Cost Spanning Tree: " + str(min_cost))
        return min_cost

    #  Display graph nodes and edges
    def printGraph(self) :
        print("\n Graph Adjacency List ", end = "")
        i = 0
        while (i < self.vertices) :
            print(" \n [", i ,"] :", end = "")
            j = 0
            #  iterate edges of i node
            while (j < len(self.adjacencylist[i])) :
                print("  ", self.adjacencylist[i][j].v, end = "")
                j += 1
            
            i += 1

    def initialize(self) :
        for i in range(int(self.vertices / 2)):
            for j in range(self.vertices - i):
                self.addEdge(j, j + i, i + j + 1)


def main() :
    g = Graph(10)
    g.addEdge(0, 1, 7)
    g.addEdge(0, 7, 6)
    g.addEdge(0, 8, 4)
    g.addEdge(1, 2, 9)
    g.addEdge(1, 8, 6)
    g.addEdge(2, 3, 8)
    g.addEdge(2, 6, 12)
    g.addEdge(2, 9, 14)
    g.addEdge(3, 4, 16)
    g.addEdge(3, 9, 5)
    g.addEdge(4, 5, 15)
    g.addEdge(5, 6, 8)
    g.addEdge(5, 9, 7)
    g.addEdge(6, 7, 2)
    g.addEdge(6, 8, 10)
    g.addEdge(8, 9, 3)
    #  Display graph element
    g.printGraph()
    g.primMST()
    g.kruskalsMST()

if __name__ == "__main__": main()