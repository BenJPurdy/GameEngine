#include "GameEngine_PCH.h"
#include "stats.h"
#include "ImGui/imgui.h"
#include "implot/implot.h"

namespace GameEngine
{
	void Stats::init()
	{
		cpuHead = 0;
		cpuCount = 0;
	}

	void Stats::onImGuiRender()
	{
		cpuBuffer[cpuHead] = process.GetCpuUsage();
		time[timeHead] = ImGui::GetTime();
		if (cpuBuffer[cpuHead] > biggestValue)
			biggestValue = cpuBuffer[cpuHead];
		cpuHead = (cpuHead+ 1) % RINGBUFFER_SIZE;
		timeHead = (timeHead+ 1) % RINGBUFFER_SIZE;
		
		if (cpuCount < RINGBUFFER_SIZE)
		{
			cpuCount++;
		}
		if (timeCount < RINGBUFFER_SIZE)
		{
			timeCount++;
		}

		double cpuTime = 0;
		for (auto& i : cpuBuffer)
		{
			cpuTime += i;
		}
		int start = cpuCount == RINGBUFFER_SIZE ? cpuHead : 0;
		double startTime = time[RINGBUFFER_SIZE ? cpuHead : 0];
		cpuTime /= (double)cpuCount;

		ImGui::Begin("Stats");
		std::string cpu = "CPU usage: " + std::to_string(cpuTime);
		ImGui::Text(cpu.c_str());
		std::string ram = "RAM usage: " + std::to_string(process.GetMemoryUsage()) + " / " + std::to_string(system.GetTotalMemory());
		ImGui::Text(ram.c_str());
		if (ImPlot::BeginPlot("CPU usage", ImVec2(400, 150), ImPlotFlags_NoTitle))
		{
			ImPlot::SetupAxes("time", "%");
			ImPlot::SetupAxesLimits(startTime, startTime + (514 * (float)1/(float)60), 0, biggestValue, ImGuiCond_Always);
			ImPlot::PlotLine("CPU usage", time.data(), cpuBuffer.data(), cpuCount);
			ImPlot::EndPlot();
		}
		
		
		

		ImGui::End();
	}
}