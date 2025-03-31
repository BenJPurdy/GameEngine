
#include "Internal/EngineInterop.h"
#include "test2.h"
//GameEngine\assets\Scripts
//GameEngine\src\Scripting\ScriptHeader
//#include "..\..\src\Scripting\ScriptHeader\ScriptHeader.h"

typedef void (*function)();
typedef void (*fnInt)(int);

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


FileWatch/FileWatcher middleware for checking updates


*/

ACCESSPOINT void TestUpdate(float ts)
{
    std::cout << "Updating\n";
    FARPROC fSound = getFunction("playSound");
    if (fSound == nullptr)
    {
        std::cout << "Failed to find function pointer for play sound\n";
    }
    else
    {
        std::cout << "Found function for playing sound\n";
    }
    fnInt func = (fnInt)(fSound);
    func(0);
}

ACCESSPOINT void testFunction()
{
    FARPROC fn = getFunction("scriptSayHello");
    if (fn == nullptr) {std::cout << "Failed to find function pointer for sayHello\n";}
    function f = (function)fn;
    f();
    FARPROC fSound = getFunction("playSound");
    if (fSound == nullptr)
    {
        std::cout << "Failed to find function pointer for play sound\n";
    }
    else
    {
        std::cout << "Found function for playing sound\n";
    }
    fnInt func = (fnInt)(fSound);
    func(0);

}

