#include "Internal/EngineInterop.h"
#include "Entity.h"

struct EnemyData
{
    int health = 100;
    float time;
};

#define CONCAT2(x, y) x ## y
#define CONCAT(one, two) CONCAT2(one, two)

#define SCRIPTNAME Enemy

#define FUNCTION(NAME)  SCRIPTAPI void CONCAT(CONCAT(SCRIPTNAME, _), NAME)
 
FUNCTION(onDestroy)(Entity e)
{
    log(LOG_TRACE, "Enemy died");
    DestroyFunc f = (DestroyFunc)getFunction("scriptDestroy");
    if (f == nullptr) return;
    delete((EnemyData*)entityData[e.handle]);
    entityData.erase(e.handle);
    f(e);
}

FUNCTION(onCollisionEnter)(Entity e, Entity other)
{
    //log(LOG_TRACE, "Entities collided");
    EnemyData& data = *(EnemyData*)entityData[e.handle];
    data.health -= 10;
    log(LOG_TRACE, "Enemy shot");
    other.destroy();
}

FUNCTION(onUpdate)(Entity e, float ts)
{
    if (!entityData.count(e.handle))
    {
        log(LOG_ERROR, "Adding enemy data to list of entity data");
        EnemyData* d = new EnemyData;
        d->time = 0.0f;
        entityData[e.handle] = (void*)d;
    }
    if (!entityData.count(e.handle))
    {
        log(LOG_ERROR, "No entity data for enemy in map");
    }
    EnemyData& data = *(EnemyData*)entityData[e.handle];
    
    if (data.health <= 0) Enemy_onDestroy(e);
}

