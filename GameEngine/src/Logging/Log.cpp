#include "GameEngine_PCH.h"

#include "Log.h"
#include "spdlog/sinks/stdout_color_sinks.h"

namespace GameEngine
{
	std::shared_ptr<spdlog::logger> Logger::logger;

	void Logger::init()
	{
		spdlog::set_pattern("%^[%T] %n: %v%$");
		logger = spdlog::stdout_color_mt("GameEngine");
		logger->set_level(spdlog::level::trace);
	}
}