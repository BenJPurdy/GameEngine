#include "Internal/EngineInterop.h"
#include "Entity.h"

struct EnemyData
{
    Entity target;
    int health = 100;
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
    if (getTag(other) == "Projectile")
    {
        EnemyData& data = *(EnemyData*)entityData[e.handle];
        data.health -= 10;
        log(LOG_TRACE, "Enemy shot");
        other.destroy();
    }
    
}

FUNCTION(onStart)(Entity e)
{
    uint32_t eID = e.handle;
    std::string msg = "Adding enemy data to list of entity data at " + std::to_string(eID);
    log(LOG_ERROR, msg);
    EnemyData* d = new EnemyData;
    Entity et = getEntity("player");
    d->target = et;
    log(LOG_ERROR, std::to_string(et.handle));
    log(LOG_ERROR, std::to_string(d->target.handle));
    //d->target = getEntity("player");
    
    //if (!d->target.exists())
    //{
    //    log(LOG_ERROR, "Failed to capture target player");
    //} 
    
    //this seems to fix issues so I'm not going to question it
    
    try
    {
       //auto ret = entityData.insert({e.handle, (void*)d});
       //msg = "RET: Map: " + std::to_string(ret.second); 
       //log(LOG_TRACE, msg);
       entityData[e.handle] = (void*)d;
    }
    catch(const std::exception& err)
    {
        std::string msg = std::string(err.what());
        log(LOG_ERROR, msg);
    }
    
    if (entityData.count(e.handle) == 0)
    {
        log(LOG_ERROR, "No entity data for enemy in map in start " + std::to_string(e.handle));
    }
    if (entityData.count(e.handle))
    {
        EnemyData* d = (EnemyData*)entityData[e.handle];
        if (d == nullptr)
        {
            log(LOG_FATAL, "entity data is nullptr");
            return;
        }
        std::string tgt = std::to_string((*(EnemyData*)entityData[e.handle]).target.handle);
        log(LOG_TRACE, "entity at e handle == " + tgt);
    }
    else{
        log(LOG_ERROR, "no entity at handle");
    }
    
}
 
FUNCTION(onUpdate)(Entity e, float ts)
{
    if (entityData.count(e.handle) == 0)
    {
        log(LOG_ERROR, "No entity data for enemy in map" + std::to_string(e.handle));

    }
    else
    {
        EnemyData& data = *(EnemyData*)entityData[e.handle];
        Transform current = e.getTransform();
        if (data.health <= 0) Enemy_onDestroy(e);
    }
    

    
    
}

