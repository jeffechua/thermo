CXX=g++
CXXFLAGS=-D NO_EMSCRIPT --std=c++17

NAMES=process gas gasNodes gasEdges jsonFactories
OBJS=${NAMES:%=obj/%.o}
DEPS=$(NAMES:%=obj/%.d)

all: test.out

obj/%.o: ../src/%.cpp
	$(CXX) $(CXXFLAGS) -o $@ -c $<

test.out: test.cpp $(OBJS)
	$(CXX) $(CXXFLAGS) -o test.out $^


clean:
	rm obj/*
	rm *.out

-include ${DEPS}