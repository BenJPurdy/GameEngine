#include "Internal/EngineInterop.h"
#include "Entity.h"

struct ProjectileData
{
    float time;
};

#define CONCAT2(x, y) x ## y
#define CONCAT(one, two) CONCAT2(one, two)

#define SCRIPTNAME Projectile

#define FUNCTION(NAME)  SCRIPTAPI void CONCAT(CONCAT(SCRIPTNAME, _), NAME)
 
FUNCTION(onDestroy)(Entity e)
{
    
    DestroyFunc f = (DestroyFunc)getFunction("scriptDestroy");
    if (f == nullptr) return;
    delete((ProjectileData*)entityData[e.handle]);
    entityData.erase(e.handle);
    f(e);
}
 
FUNCTION(onStart)(Entity e)
{
    
}

FUNCTION(onCollisionEnter)(Entity e)
{
    e.destroy();
}

FUNCTION(onUpdate)(Entity e, float ts)
{
    if (!entityData.count(e.handle))
    {
        log(LOG_ERROR, "Adding projectile data to list of entity data");
        ProjectileData* d = new ProjectileData;
        d->time = 0.0f;
        entityData[e.handle] = (void*)d;
    }
    if (!entityData.count(e.handle))
    {
        log(LOG_ERROR, "No entity data for projectile in map");
    }
    ProjectileData& data = *(ProjectileData*)entityData[e.handle];
    data.time += ts;
    if (data.time >= 1.0f)
    {
        log(LOG_TRACE, "deleting projectile");
        Projectile_onDestroy(e);
    } 
        
}

