CXX=g++
CXXFLAGS=--std=c++17 -MMD

OBJS=process.o gasNodes.o gasEdges.o jsonFactories.o interop.o

all: thermo.dylib

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -o $@ -c $<

thermo.dylib: $(OBJS)
	$(CXX) $(CXXFLAGS) -dynamiclib -fPIC -o $@ $^

clean:
	rm -f *.o
	rm -f *.d

-include $(OBJS:.o=.d)