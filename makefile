CXX=emcc
CXXFLAGS=--bind -MMD

NAMES=process gas gasNodes gasEdges jsonFactories
OBJS=${NAMES:%=obj/%.o}
DEPS=$(NAMES:%=obj/%.d)

all: build/thermo.js

obj/%.o: src/%.cpp
	$(CXX) $(CXXFLAGS) -o $@ -c $<

build/thermo.js: $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^

test: test.cpp $(OBJS)
	g++ -D NO_EMSCRIPT --std=c++17 -o test.out $^

clean:
	rm obj/*
	rm build/thermo.js
	rm build/*.wasm

-include ${DEPS}