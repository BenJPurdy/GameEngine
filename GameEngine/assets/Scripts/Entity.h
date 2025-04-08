#pragma once
#include <unordered_map>
#include "Internal/EngineInterop.h"
#include "Internal/Types.h"

extern std::unordered_map<uint32_t, void*> entityData;

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

    void playSound()
    {
        VoidEntityFunc f = (VoidEntityFunc)getFunction("playComponentSound");
        if (f == nullptr) {log(LOG_ERROR, "didn't find playSound"); return;};
        f(*this);
    }

    void addComponent(ComponentType t)
    {
        AddComponentFunc f = (AddComponentFunc)getFunction("scriptAddComponent");
        if (f == nullptr) return;
        f(*this, t);
    }

    void destroy()
    {
        DestroyFunc f = (DestroyFunc)getFunction("scriptDestroy");
        if (f == nullptr) return;
        f(*this);
    }

    void setColliderProperties(ColliderProperties props)
    {
        SetColliderPropertiesFunc f = (SetColliderPropertiesFunc)getFunction("scriptSetColliderProperties");
        if (f == nullptr) return;
        f(*this, props);
    }

    void setScript(std::string name)
    {
        SetScriptNameFunc f = (SetScriptNameFunc)getFunction("scriptAssignScriptName");
        if (f == nullptr) return;
        f(*this, name.c_str());
    }

    //copies the rigidbody properties from the entity passed into the function onto the calling entity
    void copyRigidbodyProperites(Entity e)
    {
        CopyRigidbodyPropsFunc f = (CopyRigidbodyPropsFunc)getFunction("scriptCopyRigidbodyProperties");
        if (f == nullptr) return;
        f(*this, e);
    }

    void setRigidbodyProperties(RigidbodyProperties p)
    {
        SetRigidbodyPropsFunc f = (SetRigidbodyPropsFunc)getFunction("scriptSetRigidbodyProperties");
        if (f == nullptr) return;
        f(*this, p);
    }
    //Behold, my [entity handle]
 
    uint32_t handle = 0;
    intptr_t scenePtr = 0;
};