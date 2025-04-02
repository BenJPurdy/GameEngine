#include "Internal/EngineInterop.h"
#include "Entity.h"

struct CameraData
{
    Entity player;
    bool hasPrinted = false;

}cameraData;
#define SCRIPTNAME Camera

#define FUNCTION(NAME)  SCRIPTAPI void SCRIPTNAME ## _ ##  NAME 


SCRIPTAPI void Camera_onStart(Entity e)
{
    cameraData.player = getEntity("player");
    std::string msg = "Player entity handle: " + std::to_string(cameraData.player.handle);
    log(LOG_TRACE, msg);
}

SCRIPTAPI void Camera_onUpdate(Entity e)
{
    if (!cameraData.hasPrinted)
    {
        std::string msg = "Found player at " + std::to_string(cameraData.player.handle);
        log(LOG_TRACE, msg);
        cameraData.hasPrinted = true;
    }
    
    Transform t = cameraData.player.getTransform();
    t.rotation = glm::vec3(0);
    e.setTransform(t);
}