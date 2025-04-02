#include "Internal/EngineInterop.h"
#include "Entity.h"


struct CameraData
{
    Entity player;
    bool hasPrinted = false;

};
#define CONCAT2(x, y) x ## y
#define CONCAT(one, two) CONCAT2(one, two)

#define SCRIPTNAME Camera

#define FUNCTION(NAME)  SCRIPTAPI void CONCAT(CONCAT(SCRIPTNAME, _), NAME)

FUNCTION(onStart)(Entity e)
{
    //std::string msg = "Entity handle: " + std::to_string(e.handle);
    //log(LOG_TRACE, msg);
    CameraData* data = new CameraData;
    data->player = getEntity("player");
    //msg = "Player entity handle: " + std::to_string(data->player.handle);
    //log(LOG_TRACE, msg);
    //entityData.emplace(e.handle, (void*)data);
    entityData[e.handle] = (void*)data;
    if (entityData.count(e.handle))
    {
        std::string msg = "Data has been added to the map ";
        log(LOG_WARN, msg);
    }
    else
    {
        log(LOG_FATAL, "did not add camera data to map");
    }

}

SCRIPTAPI void Camera_onUpdate(Entity e, float ts)
{
    
    
    if (!entityData.count(e.handle))
    {
        log(LOG_ERROR, "Failed to find camera data in map");
        return;
    }
    
    CameraData& data = *(CameraData*)entityData.at(e.handle);
    if (!data.hasPrinted)
    {
        std::string msg = "Camera Entity handle: " + std::to_string(e.handle);
        log(LOG_WARN, msg);
        std::string msg2 = "Found player at " + std::to_string(data.player.handle);
        log(LOG_TRACE, msg2);
        data.hasPrinted = true;
    }
    
    Transform t = data.player.getTransform();
    t.rotation = glm::vec3(0);
    e.setTransform(t);
}