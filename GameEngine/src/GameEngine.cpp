#include "GameEngine_PCH.h"

#include "GameEngine.h"

#include "Core/EntryPoint.h"
/*
int main()
{

    auto frameTime = 16'000'000ll; // 1/60 s in ns
    auto nextFrame = 0ll;
    auto now = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::high_resolution_clock::now().time_since_epoch()).count();


    //cheaper way of frame limiting than the other cursed way
    if (now >= nextFrame)
    {
        nextFrame = ((nextFrame / frameTime) + 1) * frameTime;
    }


    std::cout << "Hello World!\n";
}
*/

namespace GameEngine
{
    class GameEngineApp : public App
    {
    public:
        GameEngineApp() : App("Game Engine Editor") {}

        ~GameEngineApp() {};
    };

    App* createApp()
    {
        return new GameEngineApp();
    }
}

