#include "Internal/EngineInterop.h"
#include "Entity.h"

struct WallData
{
    bool currentlyPlayingSound = false;
    float time = 0;
};

#define CONCAT2(x, y) x ## y
#define CONCAT(one, two) CONCAT2(one, two)

#define SCRIPTNAME Wall

#define FUNCTION(NAME)  SCRIPTAPI void CONCAT(CONCAT(SCRIPTNAME, _), NAME)

FUNCTION(onStart)(Entity e)
{
    uint32_t eID = e.handle;
    std::string msg = "Adding wall data to list of entity data at " + std::to_string(eID);
    log(LOG_ERROR, msg);
    WallData* d = new WallData;
    //Entity et;
    
    //d->target = et;
    //log(LOG_ERROR, std::to_string(et.handle));
    
    //d->target = getEntity("player");
    
    //if (!d->target.exists())
    //{
    //    log(LOG_ERROR, "Failed to capture target player");
    //} 
    
    //this seems to fix issues so I'm not going to question it
    entityData[e.handle] = (void*)d;
}

FUNCTION(onCollisionEnter)(Entity e)
{
    WallData& d = *(WallData*)entityData[e.handle];
    log(LOG_TRACE, "Collided with wall");
    if (!d.currentlyPlayingSound)
    {
        e.playSound();
        d.currentlyPlayingSound = true;
    }
        
}

FUNCTION(onUpdate)(Entity e, float ts)
{
    WallData& d = *(WallData*)entityData[e.handle];
    d.time += ts;
}
