#include <iostream>
#include <climits>
#include <cstdlib> /* 亂數相關函數 */
#include <ctime>   /* 時間相關函數 */
#include <vector>
#include <algorithm>

#include <pybind11/pybind11.h>

using namespace std;
namespace pybind = pybind11;


class DSU {
    public:
        DSU(size_t n);
        size_t find(size_t i);             // Find function
        void unite(size_t x, size_t y); // union function

    private:
        ssize_t* parent;
        size_t* rank;
};


class Edge
{
    public:
        // Source node
        size_t u;
        // Destination node
        size_t v;
        // Edge weight
        size_t weight;
        Edge(size_t u, size_t v, size_t weight)
        {
            this->u = u;
            this->v = v;
            this->weight = weight;
        }
};

class HeapNode
{
public: 
    ssize_t vertex;
    ssize_t key;
};

class ResultSet
{
public: 
    size_t parent;
    size_t weight;
};

class MinHeap
{
public: 
    HeapNode **node;
    size_t *indexes;

    MinHeap(size_t capacity);
    void swap(size_t a, size_t b);
    bool isEmpty()
    {
        return this->currentSize == 0;
    }
    size_t heapSize()
    {
        return this->currentSize;
    }
    void bubbleUp(size_t pos);
    void insert(HeapNode *x);
    HeapNode *extractMin();
    void sinkDown(size_t k);

private:
    size_t capacity;
    size_t currentSize;
};

class Graph
{
    public:
        Graph(size_t vertices);
        // Graph(size_t const vertices, vector<vector>size_t> const edgelist);
        ~Graph()
        {
            adjacencylist.clear();
            vertices = 0;
        }

        void addEdge(size_t x, size_t y, size_t w);
        void decreaseKey(MinHeap *minHeap, size_t newKey, size_t vertex);
        size_t nvertices() const { return vertices; }
        vector<vector<size_t>> get_edgelist() const { return edgelist; }
        friend void initialize(Graph & g);
        friend void initialize_rand(Graph & g);
        friend size_t primMST(Graph & g);        
        friend size_t kruskalsMST(Graph & g);
        friend void printGraph(Graph & g);  // Display graph nodes and edges

    private:
        void reset_graph(size_t vertices);
        size_t vertices;
        vector < vector < Edge *> > adjacencylist;
        vector < vector < size_t> > edgelist;
};

// bool graph_connected(Graph const & graph);
void initialize(Graph & g);
void initialize_rand(Graph & g);
size_t primMST(Graph & g);  
size_t kruskalsMST(Graph & g);
void printGraph(Graph & g);  // Display graph nodes and edges



PYBIND11_MODULE(_MST, m)
{
    m.def("initialize", &initialize);
    m.def("initialize_rand", &initialize_rand);
    m.def("primMST", &primMST);
    m.def("kruskalsMST", &kruskalsMST);
    m.def("printGraph", &printGraph);

    pybind::class_<Graph>(m, "Graph")
        .def(pybind::init<size_t>())
        .def("addEdge", &Graph::addEdge)
        .def("decreaseKey", &Graph::decreaseKey);
}