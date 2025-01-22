#pragma once

namespace GameEngine
{
	class Timestep
	{
	public:
		Timestep(float time = 0.0f) : time(time) {}

		operator float() const { return time; }
		float getSeconds() const { return time; }
		float getMillliseconds() const { return time * 1000.f; }
	private:
		float time;
	};
}
