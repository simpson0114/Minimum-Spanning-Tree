CXX := g++
CXXFLAGS := -O3 -shared -std=c++17 -fPIC
OUT = _MST$(shell python3-config --extension-suffix)
PWD = $(shell pwd)

INC = -I/usr/include/python3.8/

_MST:
	$(CXX) $(CXXFLAGS) -Ofast `python3 -m pybind11 --includes` MST.cpp -o $(OUT) $(INC)

.PHONY: test clean

test:
	python3 -m pytest

clean:
	rm -rf *.so __pycache__ .pytest_cache *.txt