#include "MST.hpp"

MinHeap::MinHeap(size_t capacity)
{
    this->capacity = capacity;
    this->node = new HeapNode*[capacity + 1];
    this->indexes = new size_t[capacity];
    this->node[0] = new HeapNode();
    this->node[0]->key = INT_MIN;
    this->node[0]->vertex = -1;
    this->currentSize = 0;
}

void MinHeap::swap(size_t a, size_t b)
{
    HeapNode *temp = this->node[a];
    this->node[a] = this->node[b];
    this->node[b] = temp;
}

void MinHeap::bubbleUp(size_t pos)
{
    size_t parentIdx = pos / 2;
    size_t currentIdx = pos;
    while (currentIdx > 0 
            && this->node[parentIdx]->key > this->node[currentIdx]->key)
    {
        HeapNode *currentNode = this->node[currentIdx];
        HeapNode *parentNode = this->node[parentIdx];
        this->indexes[currentNode->vertex] = parentIdx;
        this->indexes[parentNode->vertex] = currentIdx;
        this->swap(currentIdx, parentIdx);
        currentIdx = parentIdx;
        parentIdx = parentIdx / 2;
    }
}

void MinHeap::insert(HeapNode *x)
{
    this->currentSize++;
    size_t idx = this->currentSize;
    this->node[idx] = x;
    this->indexes[x->vertex] = idx;
    this->bubbleUp(idx);
}

HeapNode *MinHeap::extractMin()
{
    HeapNode *min = this->node[1];
    HeapNode *lastNode = this->node[this->currentSize];
    this->indexes[lastNode->vertex] = 1;
    this->node[1] = lastNode;
    this->node[this->currentSize] = NULL;
    this->sinkDown(1);
    this->currentSize--;
    return min;
}

void MinHeap::sinkDown(size_t k)
{
    size_t smallest = k;
    // left child
    size_t leftChild = 2 *k;
    // right child
    size_t rightChild = 2 *k + 1;
    if (leftChild < this->heapSize() 
        && this->node[smallest]->key > this->node[leftChild]->key)
    {
        smallest = leftChild;
    }
    if (rightChild < this->heapSize() 
        && this->node[smallest]->key > this->node[rightChild]->key)
    {
        smallest = rightChild;
    }
    if (smallest != k)
    {
        HeapNode *smallestNode = this->node[smallest];
        HeapNode *kNode = this->node[k];
        this->indexes[smallestNode->vertex] = k;
        this->indexes[kNode->vertex] = smallest;
        this->swap(k, smallest);
        this->sinkDown(smallest);
    }
}


DSU::DSU(size_t n)
{
    parent = new ssize_t[n];
    rank = new size_t[n];

    for (size_t i = 0; i < n; i++) {
        parent[i] = -1;
        rank[i] = 1;
    }
}

// Find function
size_t DSU::find(size_t i)
{
    if (parent[i] == -1)
        return i;

    return parent[i] = find(parent[i]);
}

// union function
void DSU::unite(size_t x, size_t y)
{
    size_t s1 = find(x);
    size_t s2 = find(y);

    if (s1 != s2) {
        if (rank[s1] < rank[s2]) {
            parent[s1] = s2;
            rank[s2] += rank[s1];
        }
        else {
            parent[s2] = s1;
            rank[s1] += rank[s2];
        }
    }
}


Graph::Graph(size_t vertices)
{
    this->vertices = vertices;
    for (size_t i = 0; i < this->vertices; i++)
    {
        this->adjacencylist.push_back( vector < Edge *> ());
    }
}

// Graph::Graph(size_t const vertices, vector<vector>size_t> const edgelist): this->vertices(vertices)
// {
//     if(!this->edgelist.empty()) {
//         this->edgelist.clear();
//     }

//     for(auto edge : edgelist)
//         this->edgelist.push_back(edge);
// }

void Graph::addEdge(size_t u, size_t v, size_t weight)
{
    this->edgelist.push_back({weight, u, v});
    this->adjacencylist.at(u).push_back(new Edge(u, v, weight));
    // cout << this->adjacencylist.at(u).size() << endl;
    if (u == v)
    {
        // self loop
        return;
    }
    this->adjacencylist.at(v).push_back(new Edge(v, u, weight));
}

void Graph::decreaseKey(MinHeap *minHeap, size_t newKey, size_t vertex)
{
    size_t index = minHeap->indexes[vertex];
    HeapNode *node = minHeap->node[index];
    node->key = newKey;
    minHeap->bubbleUp(index);
}

size_t kruskalsMST(Graph & g)
{
    // 1. Sort all edges
    sort(g.edgelist.begin(), g.edgelist.end());

    // Initialize the DSU
    DSU s(g.vertices);
    size_t min_cost = 0;
    cout << "\n\n Minimum Spanning Tree (Kruskal's) \n";
    for (auto edge : g.edgelist) {
        size_t w = edge[0];
        size_t x = edge[1];
        size_t y = edge[2];

        // take that edge in MST if it does form a cycle
        if (s.find(x) != s.find(y)) {
            s.unite(x, y);
            min_cost += w;
            cout << "Edge (" << x << "-" << y  << ")  weight : " << w
                << endl;
        }
    }
    cout << "Minimum Cost Spanning Tree: " << min_cost << endl;
    return min_cost;
}

size_t primMST(Graph & g)
{
    bool inHeap[g.vertices];
    ResultSet *resultSet[g.vertices];
    size_t key[g.vertices];
    HeapNode *heapNodes[g.vertices];
    // Set default value of heap nodes resultset and keys
    for (size_t i = 0; i < g.vertices; i++)
    {
        heapNodes[i] = new HeapNode();
        heapNodes[i]->vertex = i;
        heapNodes[i]->key = INT_MAX;
        resultSet[i] = new ResultSet();
        resultSet[i]->parent = -1;
        inHeap[i] = true;
        key[i] = INT_MAX;
    }
    heapNodes[0]->key = 0;
    MinHeap *minHeap = new MinHeap(g.vertices);
    for (size_t j = 0; j < g.vertices; j++)
    {
        minHeap->insert(heapNodes[j]);
    }
    size_t i = 0;
    while (minHeap->isEmpty() == false)
    {
        HeapNode *extractedNode = minHeap->extractMin();
        size_t extractedVertex = extractedNode->vertex;
        inHeap[extractedVertex] = false;
        while (i < g.adjacencylist.at(extractedVertex).size())
        {
            Edge *edge = g.adjacencylist.at(extractedVertex).at(i);
            if (inHeap[edge->v])
            {
                size_t v = edge->v;
                size_t w = edge->weight;
                if (key[v] > w)
                {
                    key[v] = w;
                    g.decreaseKey(minHeap, w, v);
                    resultSet[v]->parent = extractedVertex;
                    resultSet[v]->weight = w;
                }
            }
            i++;
        }
        i = 0;
    }
    size_t result = 0;
    cout << "\n\n Minimum Spanning Tree (Prim's) \n";
    for (size_t node = 1; node < g.vertices; ++node)
    {
        cout << " Edge (" << resultSet[node]->parent 
                << "-" << node << ")  weight : " 
                << resultSet[node]->weight << endl;
        result += resultSet[node]->weight;
    }
    // Display calculated result
    cout << " Total minimum weight : " << result << endl;
    return result;
}

// Display graph nodes and edges
void printGraph(Graph & g)
{
    cout << "\n Graph Adjacency List ";
    for (size_t i = 0; i < g.vertices; i++)
    {
        cout << " \n [" << i << "] :";
        // cout << this->adjacencylist.at(i).size() << endl;
        // iterate edges of i node
        for (size_t j = 0; j < g.adjacencylist.at(i).size(); j++)
        {
            cout << "  " << g.adjacencylist.at(i).at(j)->v;
        
        }
    }
}

/*
// Function to perform DFS traversal on the graph on a graph
void DFS(Graph const &graph, size_t v, vector<bool> &visited)
{
    // mark current node as visited
    visited[v] = true;
 
    // do for every edge (v, u)
    for (size_t : graph.adjList[v])
    {
        // `u` is not visited
        if (!visited[u]) {
            DFS(graph, u, visited);
        }
    }
}
*/

/*
bool graph_connected(Graph const & graph) {
    // do for every vertices
    size_t parent[graph.nvertices()];
    size_t i;
    for (i = 0; i < graph.nvertices(); i++)
        parent[i] = i;

    for (auto edge : graph.get_edgelist())
        parent[edge[1]] = parent[edge[0]];

    for (i = 0; i < graph.nvertices(); i++) {
        if(parent[i] != 0)
            throw std::domain_error("the graph is not strong connected");
    }
    
}
*/

void initialize(Graph & g) {
    for(size_t i = 1; i < g.vertices / 2; i += g.vertices / 8) {
        for(size_t j = 0; j + i < g.vertices; j += i) {
            g.addEdge(j, j + i, i + j);
        }
    }
}

void initialize_rand(Graph & g) {
    srand( time(NULL) );
    for(size_t i = 1; i < g.vertices / 2; i += g.vertices / 8) {
        for(size_t j = 0; j + i < g.vertices; j += i) {
            g.addEdge(j, j + i, rand() % 100 + 1);
        }
    }
}

/*
int main()
{
    Graph *g = new Graph(10);
    initialize(*g);
    
    // g->addEdge(0, 1, 7);
    // g->addEdge(0, 7, 6);
    // g->addEdge(0, 8, 4);
    // g->addEdge(1, 2, 9);
    // g->addEdge(1, 8, 6);
    // g->addEdge(2, 3, 8);
    // g->addEdge(2, 6, 12);
    // g->addEdge(2, 9, 14);
    // g->addEdge(3, 4, 16);
    // g->addEdge(3, 9, 5);
    // g->addEdge(4, 5, 15);
    // g->addEdge(5, 6, 8);
    // g->addEdge(5, 9, 7);
    // g->addEdge(6, 7, 2);
    // g->addEdge(6, 8, 10);
    // g->addEdge(8, 9, 3);
    
    // Display graph element
    printGraph(*g);
    primMST(*g);
    kruskalsMST(*g);
    return 0;
}
*/
