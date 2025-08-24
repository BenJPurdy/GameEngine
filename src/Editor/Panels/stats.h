#pragma once

#include "sysinfo/include/ProcessInfo.h"
#include "sysinfo/include/SystemInformation.h"
#include <array>
#define RINGBUFFER_SIZE 512

namespace GameEngine
{
	class Stats
	{
	public:
		Stats() = default;
		ProcessInfo process;
		SystemInformation system;
		double biggestValue = 0;

		std::array<double, RINGBUFFER_SIZE> cpuBuffer{};
		size_t cpuHead = 0;
		size_t cpuCount = 0;
		std::array<double, RINGBUFFER_SIZE> time{};
		size_t timeHead = 0;
		size_t timeCount = 0;

		void init();

		void onImGuiRender();

	};
}
