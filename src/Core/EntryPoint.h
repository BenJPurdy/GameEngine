#pragma once

#include "Core/Core.h"
#include "Core/App.h"

extern GameEngine::App* GameEngine::createApp();

int main(int argc, char** argv)
{

	GameEngine::Logger::init();

	printf("GameEngine\n");

	auto app = GameEngine::createApp();

	app->run();

	delete app;
}

extern "C"
{
	_declspec(dllexport) DWORD NvOptimusEnablement = 1;
	_declspec(dllexport) int AmdPowerXpressRequestHighPerformance = 1;
}
