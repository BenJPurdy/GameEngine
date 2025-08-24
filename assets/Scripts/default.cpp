#include "Internal/EngineInterop.h"
#include "Entity.h"

struct ENTITYNAMEData
{
    float time;
};

#define CONCAT2(x, y) x ## y
#define CONCAT(one, two) CONCAT2(one, two)

#define SCRIPTNAME ENTITYNAME

#define FUNCTION(NAME)  SCRIPTAPI void CONCAT(CONCAT(SCRIPTNAME, _), NAME)
