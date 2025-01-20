// GameEngine.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <chrono>

int main()
{

    auto frameTime = 16'000'000ll; // 1/60s
    auto nextFrame = 0ll;
    auto now = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::high_resolution_clock::now().time_since_epoch()).count();


    //cheaper way of frame limiting than the other cursed way
    if (now >= nextFrame)
    {
        nextFrame = ((nextFrame / frameTime) + 1) * frameTime;
    }


    std::cout << "Hello World!\n";
}


