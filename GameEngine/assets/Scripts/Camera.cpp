#include "Internal/EngineInterop.h"
#include "Entity.h"

struct CameraData
{
    Entity player;

}cameraData;
#define SCRIPTNAME Camera

#define FUNCTION(NAME)  SCRIPTAPI void SCRIPTNAME ## _ ##  NAME 


SCRIPTAPI void Camera_onStart(Entity e)
{
    cameraData.player = getEntity("player");
}

SCRIPTAPI void Camera_onUpdate(Entity e)
{
    Transform t = cameraData.player.getTransform();
    t.rotation = glm::vec3(0);
    e.setTransform(t);
}