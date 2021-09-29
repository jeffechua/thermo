#include <fstream>
#include <iostream>
#include <sstream>
#include "../src/process.hpp"

using namespace std;

int main() {
    ifstream stream("../build/sampleProcess.json");
    stringstream buffer;
    buffer << stream.rdbuf();
    Process process(buffer.str());
}