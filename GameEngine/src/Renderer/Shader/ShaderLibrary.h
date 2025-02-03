#pragma once
#include <string>
#include <unordered_map>
#include <glm/glm.hpp>
#include "Renderer/Shader/Shader.h"

typedef unsigned int GLenum;

namespace GameEngine
{
	class ShaderLibrary
	{
	public:
		void add(const Ref<Program>&);
		void add(const std::string&, const Ref<Program>&);

		Ref<Program> load(const std::string&);
		Ref<Program> load(const std::string&, const std::string&);

		Ref<Program> get(const std::string&);

		bool exists(const std::string&) const;

	private:
		std::unordered_map<std::string, Ref<Program>> shaders;
	};
}
