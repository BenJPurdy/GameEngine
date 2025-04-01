#include "GameEngine_PCH.h"
#include "ScriptInteface.h"

SCRIPT_API std::vector<Interface*> registry;

SCRIPT_API void registerScript(Interface* script)
{
    registry.push_back(script);
}