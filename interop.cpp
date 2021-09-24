#include "interop.h"

#include <fstream>
#include <sstream>

#include "json.hpp"

using namespace std;
using namespace nlohmann;

IopGasState& IopGasState::operator=(const GasState& state) {
    P = state.P;
    T = state.T;
    V_ = state.V_;
    averageMolarMass = state.averageMolarMass;
    density = state.density;
    physH_ = state.physH_;
    chemH_ = state.chemH_;
    totalH_ = state.totalH_;
    for (int i = 0; i < state.xs.size() && i < 10; i++) xs[i] = state.xs[i];
    return *this;
}
IopGasElement& IopGasElement::operator=(const GasElement& element) {
    n = element.n;
    physH = element.physH;
    for (int i = 0; i < element.ns.size() && i < 10; i++) ns[i] = element.ns[i];
    return *this;
}

void* initProcess(const char* path) {
    ifstream stream(path);
    stringstream buffer;
    buffer << stream.rdbuf();
    stream.close();
    return new Process(json::parse(buffer.str()));
}

void tickProcess(void* process, int ticks) {
    Process* p = static_cast<Process*>(process);
    for (int i = 0; i < ticks; i++) p->tick();
}

void endProcess(void* process) { delete static_cast<Process*>(process); }

void readProcessBindings(void* process) {
    static_cast<Process*>(process)->writeToBindings();
}

void bindNodeState(void* process, char* name, void* view) {
    static_cast<Process*>(process)->bindNode(name, static_cast<IopGasState*>(view));
}

void bindNodeElement(void* process, char* name, void* view) {
    static_cast<Process*>(process)->bindNode(name, static_cast<IopGasElement*>(view));
}

void bindEdgeState(void* process, char* name, void* view) {
    static_cast<Process*>(process)->bindEdge(name, static_cast<IopGasState*>(view));
}

void bindEdgeElement(void* process, char* name, void* view) {
    static_cast<Process*>(process)->bindEdge(name, static_cast<IopGasElement*>(view));
}