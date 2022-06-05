import _MST
import pyMST
import pytest
import timeit
import time

def graph_init(vertices):
    g = _MST.Graph(vertices)
    _MST.initialize(g)
    return g

def graph_init_rand(vertices):
    g = _MST.Graph(vertices)
    _MST.initialize_rand(g)
    return g

def py_graph_init(vertices):
    py_g = pyMST.Graph(vertices)
    py_g.initialize()
    return py_g



def test_difmethod():
    g_10 = graph_init_rand(10)
    g_100 = graph_init_rand(100)
    g_1000 = graph_init_rand(1000)
    g_200000 = graph_init_rand(200000)
    # py_g = py_graph_init(g_10)
    assert _MST.primMST(g_10) == _MST.kruskalsMST(g_10)
    assert _MST.primMST(g_100) == _MST.kruskalsMST(g_100)
    assert _MST.primMST(g_1000) == _MST.kruskalsMST(g_1000)
    assert _MST.primMST(g_200000) == _MST.kruskalsMST(g_200000)

def test_diflang():
    g_1000 = graph_init(1000)
    pyg_1000 = py_graph_init(1000)
    assert _MST.primMST(g_1000) == _MST.kruskalsMST(g_1000) == pyg_1000.primMST() == pyg_1000.kruskalsMST()

def test_time():
    g_1000 = graph_init(1000)
    g_200000 = graph_init(200000)
    py_g_1000 = py_graph_init(1000)
    # py_g_10000 = py_graph_init(10000)

    init = dict(_MST=_MST, pyMST = pyMST, g_1000 = g_1000, g_200000 = g_200000, py_g_1000 = py_g_1000, tsize=10)
    prim_1000 = timeit.timeit(
        "_MST.primMST(g_1000)", number=10, globals=init)
    kruskal_1000 = timeit.timeit(
        "_MST.kruskalsMST(g_1000)", number=10, globals=init)
    prim_200000 = timeit.timeit(
        "_MST.primMST(g_200000)", number=10, globals=init)
    kruskal_200000 = timeit.timeit(
        "_MST.kruskalsMST(g_200000)", number=10, globals=init)
    pyprim_1000 = timeit.timeit(
        "py_g_1000.primMST()", number=10, globals=init)
    pykruskal_1000 = timeit.timeit(
        "py_g_1000.kruskalsMST()", number=10, globals=init)
    # pyprim_2000 = timeit.timeit(
    #     "py_g_10000.primMST()", number=10, globals=init)
    # pykruskal_2000 = timeit.timeit(
    #     "py_g_10000.kruskalsMST()", number=10, globals=init)

    with open("performance.txt", "w") as f:
        f.write(f"primMST_1000: {prim_1000 / 10:.4f} seconds.\n")
        f.write(f"kruskalsMST_1000: {kruskal_1000 / 10:.4f} seconds.\n")
        f.write(f"primMST_200000: {prim_200000 / 10:.4f} seconds.\n")
        f.write(f"kruskalsMST_200000: {kruskal_200000 / 10:.4f} seconds.\n")
        f.write(f"pyprim_1000: {pyprim_1000 / 10:.4f} seconds.\n")
        f.write(f"pykruskal_1000: {pykruskal_1000 / 10:.4f} seconds.\n")
        # f.write(f"pyprim_2000: {pyprim_2000 / 10:.4f} seconds.\n")
        # f.write(f"pykruskal_2000: {pykruskal_2000 / 10:.4f} seconds.\n")