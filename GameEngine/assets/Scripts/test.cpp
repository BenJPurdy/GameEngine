
#include "Internal/EngineInterop.h"
#include "test2.h"
//GameEngine\assets\Scripts
//GameEngine\src\Scripting\ScriptHeader
//#include "..\..\src\Scripting\ScriptHeader\ScriptHeader.h"

typedef void (*function)();

#define ACCESSPOINT extern "C"

/*

class Entity
{
    void setPosition(Transform)
        scriptSetPosition(*this, transform)

    private:
    uint32_t handle;
    uint64_t sceneHandle;
}

onUpdate(Entity e)
{
    e.setPosition(t);
}

//in update
id = 
...
Transform t;
t.position += 0.1f;

setTransform(t);



*/

ACCESSPOINT void testFunction()
{
    FARPROC fn = getFunction("scriptSayHello");
    if (fn == nullptr) {std::cout << "Failed to find function pointer for sayHello";}
    function f = (function)fn;
    f();
}

