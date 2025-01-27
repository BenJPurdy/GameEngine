#pragma once

#include "Core/Core.h"
#include "spdlog/spdlog.h"
#include "spdlog/fmt/ostr.h"

namespace GameEngine
{
	class Logger
	{
	public:
		static void init();
		inline static std::shared_ptr<spdlog::logger>& getLogger() { return logger; }

	private:
		static std::shared_ptr<spdlog::logger> logger;
	};
}

#define LOG_TRACE(...) :: GameEngine::Logger::getLogger()->trace(__VA_ARGS__)
#define LOG_INFO(...)  :: GameEngine::Logger::getLogger()->info(__VA_ARGS__)
#define LOG_WARN(...)  :: GameEngine::Logger::getLogger()->warn(__VA_ARGS__)
#define LOG_ERROR(...) :: GameEngine::Logger::getLogger()->error(__VA_ARGS__)
#define LOG_FATAL(...) :: GameEngine::Logger::getLogger()->critical(__VA_ARGS__)
