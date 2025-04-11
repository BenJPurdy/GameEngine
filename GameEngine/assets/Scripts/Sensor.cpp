#include "Internal/EngineInterop.h"
#include "Entity.h"

struct SensorData
{
    bool canActivate = false;
    float time;
}data;

#define CONCAT2(x, y) x ## y
#define CONCAT(one, two) CONCAT2(one, two)

#define SCRIPTNAME Sensor

#define FUNCTION(NAME)  SCRIPTAPI void CONCAT(CONCAT(SCRIPTNAME, _), NAME)



FUNCTION(onCollisionStart)(Entity e, Entity other)
{
    if (data.canActivate)
    {log(LOG_TRACE, "win condition");}
}


