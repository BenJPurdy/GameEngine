#include "Internal/Registry.h"


#include <iostream>

void onStart()
{
    std::cout << "On Start called";
}

void onUpdate()
{
    std::cout << "calling on update, on update";
}

void onDestroy()
{
    std::cout << "Destorying object";
}

Interface scriptInterface = 
{
    "default", //replace with script name
    onStart,
    onUpdate,
    onDestroy
};

struct AutoRegister {
    AutoRegister(){scriptInterface.registerScript(&scriptInterface);}
}autoRegister;