#include "Internal/EngineInterop.h"
#include "Entity.h"

struct WallData
{
    
};

#define CONCAT2(x, y) x ## y
#define CONCAT(one, two) CONCAT2(one, two)

#define SCRIPTNAME Wall

#define FUNCTION(NAME)  SCRIPTAPI void CONCAT(CONCAT(SCRIPTNAME, _), NAME)


FUNCTION(onCollisionEnter)(Entity e)
{
    log(LOG_TRACE, "Collided with wall");
    e.playSound();
}

FUNCTION(onUpdate)(Entity e)
{

}
