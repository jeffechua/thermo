#include <algorithm>
#include <fstream>
#include <iostream>
#include <sstream>

#include "json.hpp"
#include "process.hpp"

using namespace std;
using namespace nlohmann;

int main() {
    ifstream stream("sampleProcess.json");
    stringstream buffer;
    buffer << stream.rdbuf();
    Process process(json::parse(buffer.str()));
    string input;
    while (getline(cin, input)) {
        cout << "main:\n";
        cout << "   " << process.nodeMap["nitrogen main"]->state.P << "\n";
        cout << "   " << process.nodeMap["nitrogen main"]->state.T << "\n";
        cout << "   " << process.nodeMap["nitrogen main"]->state.density << "\n";
        cout << "flow:\n";
        cout << "   " << process.edgeMap["line to cylinder"]->flow.n << "\n";
        cout << "cylinder:\n";
        cout << "   " << process.nodeMap["cylinder"]->contents.n << "\n";
        cout << "   " << process.nodeMap["cylinder"]->state.P << "\n";
        cout << "   " << process.nodeMap["cylinder"]->state.T << "\n";
        cout << "   " << process.nodeMap["cylinder"]->state.density << "\n";
        cout << "flow:\n";
        cout << "   " << process.edgeMap["line to tank"]->flow.n << "\n";
        cout << "tank:\n";
        cout << "   " << process.nodeMap["tank"]->contents.n << "\n";
        cout << "   " << process.nodeMap["tank"]->state.P << "\n";
        cout << "   " << process.nodeMap["tank"]->state.T << "\n";
        cout << "   " << process.nodeMap["tank"]->state.density << "\n";
        process.tick();
    }
}