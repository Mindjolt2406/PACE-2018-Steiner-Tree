# CPP flags
CXX = g++
CXXFLAGS = -std=c++14 -g -Wall
CPPFLAGS = -I eigen
LDLIBS   = -lm

# Custom aliases
NTD = NiceTreeDecomposition

main: main.o NiceTree.o
	$(CXX) $(CXXFLAGS) $(LDLIBS) -o main main.o NiceTree.o

NiceTree.o : $(NTD)/NiceTree.hpp $(NTD)/NiceTree.cpp
	$(CXX) $(CXXFLAGS) -c $(NTD)/NiceTree.cpp

main.o : main.cpp $(NTD)/NiceTree.hpp
	$(CXX) $(CXXFLAGS) -c main.cpp 

clean:
	rm -rf *.o main