#pragma once

#include "Internal/EngineInterop.h"
#include "Internal/Types.h"


class Entity
{  
    public:
    Entity() = default;
    
    Entity(uint32_t _handle, uint64_t _scenePtr) : handle(_handle), scenePtr(_scenePtr)
    {}

    void setTransform(Transform t)
    {
        SetTransformFunc f = (SetTransformFunc)getFunction("scriptSetTransform");
        if (f == nullptr) {log(LOG_ERROR, "didn't find setTransform");}
        f(*this, t);

    }

    void addForce(glm::vec3 F)
    {
        AddForceFunc f = (AddForceFunc)getFunction("scriptAddForce");
        f(*this, F);
    }

    Transform getTransform()
    {
        GetTransformFunc f = (GetTransformFunc)getFunction("scriptGetTransform");
        if (f == nullptr) {log(LOG_ERROR, "didn't find getTransform");}
        return f(*this);
    }



    //Behold, my [entity handle]
 
    uint32_t handle = 0;
    intptr_t scenePtr = 0;
};