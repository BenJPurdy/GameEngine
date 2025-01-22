#pragma once

#include "Core/Core.h"
#include "Core/App.h"

extern GameEngine::App* GameEngine::createApp();

int main(int argc, char** argv)
{
	printf("GameEngine\n");

	auto app = GameEngine::createApp();

	app->run();

	delete app;
}
