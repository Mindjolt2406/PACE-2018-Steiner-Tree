# CPP flags
CXX = g++
CXXFLAGS = -std=c++14 -g -Wall
CPPFLAGS = -I eigen
LDLIBS   = -lm

# Custom aliases
NTD = NiceTreeDecomposition
REC = Recurrence

main: main.o NiceTree.o Recurrence.o Partition.o
	$(CXX) $(CXXFLAGS) $(LDLIBS) -o main main.o NiceTree.o Recurrence.o Partition.o

NiceTree.o: $(NTD)/NiceTree.hpp $(NTD)/NiceTree.cpp
	$(CXX) $(CXXFLAGS) -c $(NTD)/NiceTree.cpp

Partition.o: $(REC)/Partition.hpp $(REC)/Partition.cpp
	$(CXX) $(CXXFLAGS) -c $(REC)/Partition.cpp

Recurrence.o: $(REC)/Recurrence.hpp $(REC)/Recurrence.cpp $(REC)/Partition.hpp
	$(CXX) $(CXXFLAGS) -c $(REC)/Recurrence.cpp

main.o: main.cpp $(NTD)/NiceTree.hpp $(REC)/Recurrence.hpp
	$(CXX) $(CXXFLAGS) -c main.cpp 

clean:
	rm -rf *.o main