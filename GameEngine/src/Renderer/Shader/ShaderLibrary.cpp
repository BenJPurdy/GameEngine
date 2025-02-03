#include "GameEngine_PCH.h"
#include "ShaderLibrary.h"
#include "Shader.h"

namespace GameEngine
{
	void ShaderLibrary::add(const std::string& name, const Ref<Program>& shader)
	{
		CORE_ASSERT(!exists(name), "Shader already exists");
		shaders[name] = shader;
	}

	void ShaderLibrary::add(const Ref<Program>& shader)
	{
		add(shader->getName(), shader);
	}

	Ref<Program> ShaderLibrary::load(const std::string& fp)
	{
		Ref<Program> shader = std::make_shared<Program>(fp);
		add(shader);
		return shader;
	}

	Ref<Program> ShaderLibrary::load(const std::string& name, const std::string& fp)
	{
		Ref<Program> shader = std::make_shared<Program>(fp);
		add(name, shader);
		return shader;
	}

	Ref<Program> ShaderLibrary::get(const std::string& name)
	{
		CORE_ASSERT(exists(name), "Shader not found");
		return shaders[name];
	}

	bool ShaderLibrary::exists(const std::string& name) const
	{
		return shaders.find(name) != shaders.end();
	}
}