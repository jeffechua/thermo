CXX=g++
CXXFLAGS=--std=c++17 -MMD

OBJS=main.o process.o gasNodes.o gasEdges.o jsonFactories.o

all: main

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -o $@ -c $<

main: $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^

clean:
	rm -f *.o
	rm -f *.d

-include $(OBJS:.o=.d)