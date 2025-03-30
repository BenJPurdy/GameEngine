#include "Internal/EngineInterop.h"
#include "Internal/Types.h"


class Entity
{

    void onStart()
    {

    }

    void setTransform(Transform t)
    {
        SetTransformFunc f = (SetTransformFunc)getFunction("scriptSetTransform");
        f(*this, t);

    }

    Transform getTransform()
    {
        GetTransformFunc f = (GetTransformFunc)getFunction("scriptGetTransform");
        return f(*this);
    }

    void onCollisionEnter()
    {

    }

    void onCollisionExit()
    {

    }

    void onDestory()
    {
        
    }

    //Behold, my [entity handle]
    private:
    const uint32_t handle;
    const uint64_t scenePtr;
}