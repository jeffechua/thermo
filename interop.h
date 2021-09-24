#pragma once
#include "gas.hpp"
#include "process.hpp"

#ifdef _WIN32
#define EXPORT __declspec(dllexport)
#else
#define EXPORT
#endif

extern "C" {
EXPORT void* initProcess(const char* path);
EXPORT void tickProcess(void* process, int ticks);
EXPORT void endProcess(void* process);
EXPORT void readProcessBindings(void* process);
EXPORT void bindNodeState(void* process, char* name, void* view);
EXPORT void bindNodeElement(void* process, char* name, void* view);
EXPORT void bindEdgeState(void* process, char* name, void* view);
EXPORT void bindEdgeElement(void* process, char* name, void* view);
}