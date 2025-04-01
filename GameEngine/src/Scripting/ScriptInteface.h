#pragma once
#include <vector>
#include <string>

#define SCRIPT_API __declspec(dllexport)



typedef void (*ScriptFunction)();

struct Interface
{
    std::string scriptName;
    ScriptFunction onStart;
    ScriptFunction onUpdate;
    ScriptFunction onDestroy;
};

SCRIPT_API void registerScript(Interface* script);
