#pragma once

#include <vector>
#include <string>

#define SCRIPT_API __declspec(dllexport)

typedef void (*ScriptFunction)();
struct Interface;
SCRIPT_API std::vector<Interface*> registry;

SCRIPT_API std::vector<Interface*> getRegistry()
{
    return registry;
}

struct Interface
{
    std::string scriptName;
    ScriptFunction onStart;
    ScriptFunction onUpdate;
    ScriptFunction onDestroy;

    SCRIPT_API void registerScript(Interface* script)
    {
        registry.push_back(script);
    }
};




