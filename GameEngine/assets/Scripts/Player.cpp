#include "Internal/EngineInterop.h"
#include "Entity.h"
#include <string>
//GameEngine\assets\Scripts
//GameEngine\src\Scripting\ScriptHeader
//#include "..\..\src\Scripting\ScriptHeader\ScriptHeader.h"


#define SCRIPTNAME MyEntity

#define FUNCTION(NAME)  ACCESSPOINT SCRIPT_API void SCRIPTNAME ## _ ##  NAME 

/*

class Entity
{
    void setPosition(Transform)
        scriptSetPosition(*this, transform)

    private:
    uint32_t handle;
    uint64_t sceneHandle;
}

onUpdate(Entity e)
{
    e.setPosition(t);
}

//in update
id = 
...
Transform t;
t.position += 0.1f;

setTransform(t);


FileWatch/FileWatcher middleware for checking updates


*/

//FUNCTION(onStart)()
//{
//
//}

static struct MyEntityData
{
    bool mouseClicked = false;
    int health = 20;
    Entity camera;
}data;


SCRIPTAPI void MyEntity_onStart(Entity e)
{
    log(LOG_TRACE, "finding camera");
    std::string find = "Main Camera";
    log(LOG_TRACE, "getting camera");
    data.camera = getEntity(find);
    log(LOG_TRACE, "got camera");
}

glm::vec3 inputHandling()
{
    glm::vec3 addPos = glm::vec3{0};
    if (GetKeyPressed(Key::A))
    {
        //log(LOG_WARN, "A key pressed");
        addPos.x -= 1.0;
    }
    if (GetKeyPressed(Key::D))
    {
        //log(LOG_WARN, "D key pressed");
        addPos.x += 1.0;
    }
    if (GetKeyPressed(Key::S))
    { 
        //log(LOG_WARN, "S key pressed");
        addPos.y -= 1.0;
    }
    if (GetKeyPressed(Key::W))
    {
        //log(LOG_WARN, "W key pressed");
        addPos.y += 1.0;
    }
    return addPos;
}


SCRIPTAPI void MyEntity_onUpdate(Entity e, float ts)
{
    
    //std::string msg = "Handle " + std::to_string(e.handle) + " scenePtr " + std::to_string(e.scenePtr);
    Transform t = e.getTransform();
    //std::string msg = t.to_string() + "  " + std::to_string(ts);
    //log(LOG_WARN, msg);

    //log(LOG_TRACE, "Updating entity");
    glm::vec3 input = inputHandling();
    
    e.addForce(input * 10.0f);
    

    
    MousePosition mPos = GetMousePos();
    mPos.x -= 0.5f;
    mPos.y -= 0.5f;
    //std::string msg = "MPOS: " + std::to_string(mPos.x) + ", " + std::to_string(mPos.y);
    //log(LOG_TRACE, msg);
    glm::vec2 mouse = glm::vec2(mPos.x, mPos.y);
    mouse = glm::normalize(mouse);
    //log(LOG_TRACE, glm::to_string(mouse));
 

    //state lock on mouse click, and we spawn a projectile
    if (GetMousePressed(ButtonLeft) && GetKeyPressed(LeftAlt) && !data.mouseClicked)
    {
        mouse = glm::vec2(mouse.x, -mouse.y);
        glm::vec3 sp = e.getTransform().position;
        sp += glm::vec3(mouse, 0.0f);
        Transform spawn;
        spawn.position = sp;
        spawn.scale = glm::vec3(0.1f); 
        log(LOG_TRACE, "Mouse clicked");
        std::string name = "Projectile";
        Entity s = newSpawnEntity(e, name);
        s.addComponent(ComponentType::ScriptComponent);
        s.setScript(name);
        s.addComponent(ComponentType::Rigidbody2dComponent);
        s.copyRigidbodyProperites(e);
        s.addComponent(ComponentType::CircleCollider2dComponent);
        s.setTransform(spawn);
        ColliderProperties collProps;
        collProps.scale = glm::vec2(0.1f);
        collProps.density = 2.0f;
        collProps.friction = 0.0f;
        s.setColliderProperties(collProps);
        s.addComponent(ComponentType::CircleRenderComponent);
        s.addForce(glm::vec3(mouse, 0.0f) * 0.5f);
        
        
        
    }
    data.mouseClicked = GetMousePressed(ButtonLeft);
}

//Entity other can be ignored but is passed in
SCRIPTAPI void MyEntity_onCollisionEnter(Entity e, Entity other)
{
    log(LOG_TRACE, "woah, collision");
    playSound(0);
    if (getTag(other) == "Projectile")
    {
        data.health -= 10;
    }
}



SCRIPTAPI void TestUpdate(float ts)
{
    log(LOG_TRACE, "Updating");
    //FARPROC fSound = getFunction("playSound");
    //if (fSound == nullptr)
    //{
    //    std::cout << "Failed to find function pointer for play sound\n";
    //}
    //else
    //{
    //    std::cout << "Found function for playing sound\n";
    //}
    //fnInt func = (fnInt)(fSound);
    //func(0);
}

ACCESSPOINT SCRIPT void testFunction()
{
    FARPROC fn = getFunction("scriptSayHello");
    if (fn == nullptr) {log(LOG_ERROR, "Failed to find function pointer for sayHello");}
    voidFunc f = (voidFunc)fn;
    f();
    FARPROC fSound = getFunction("playSound");
    if (fSound == nullptr)
    {
        log(LOG_ERROR, "Failed to find function pointer for play sound");
    }
    else
    {
        log(LOG_ERROR, "Found function for playing sound");
    }
    intFunc func = (intFunc)(fSound);
    //func(0);

}

