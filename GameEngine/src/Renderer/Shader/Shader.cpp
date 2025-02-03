#include "GameEngine_PCH.h"
#include "Shader.h"

#include <glm/gtc/type_ptr.hpp>
#include <gl3w.h>
#include <fstream>
#include <vector>

#include <filesystem>
#include "Utilities//FileManagement.h"

#define VALID_SHADER_FILE_MIN_COUNT 2
#define VALID_SHADER_FILE_MAX_COUNT 2

namespace GameEngine
{

	static GLenum shaderTypeFromString(const std::string& t)
	{
		if (t == "vertex") { return GL_VERTEX_SHADER; }
		else if (t == "fragment" || t == "pixel") { return GL_FRAGMENT_SHADER; }

		CORE_ASSERT(false, "UNK_SHADER");
		return 0;
	}

	Program::Program(const std::string& fp)
	{
		namespace FM = FileManagment;
		std::vector<std::pair<std::string, std::string>> filePaths;
		if (!FM::getFilesInFolder(&filePaths, fp))
		{
			LOG_ERROR("didn't read anything");
			return;
		}
		if (filePaths.size() < VALID_SHADER_FILE_MIN_COUNT ||
			filePaths.size() > VALID_SHADER_FILE_MAX_COUNT)
		{
			LOG_ERROR("incorrect number of files in shader dir {0}", fp);
		}

		std::string frag, vert;
		for (auto& entry : filePaths)
		{
			if (entry.second == ".frag") frag = entry.first;
			else if (entry.second == ".vert") vert = entry.first;
		}

		id = glCreateProgram();
		std::vector<char> vertexCode, fragmentCode;
		if (!FM::readFile(vert, &vertexCode)) LOG_ERROR("Failed to read vertex shader");
		if (!FM::readFile(frag, &fragmentCode)) LOG_ERROR("Failed to read fragment shader");
		
		auto vertShader = compileShader(GL_VERTEX_SHADER, vertexCode);
		auto fragShader = compileShader(GL_FRAGMENT_SHADER, fragmentCode);

	}
		
	bool Program::compileStatus(GLuint shader)
	{
		GLint result = GL_FALSE;
		int type = 0, logLength = 0, srcLength = 0;
		if (glIsShader(shader)) { glGetShaderiv(shader, GL_SHADER_TYPE, &type); }
		else if (glIsProgram(shader)) { type = GL_PROGRAM; }

		if (type == GL_PROGRAM) { glGetProgramiv(shader, GL_LINK_STATUS, &result); }
		else { glGetShaderiv(shader, GL_COMPILE_STATUS, &result); }

		if (result == GL_FALSE)
		{
			if (type == GL_PROGRAM)
			{
				glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logLength);
				std::string shaderError((logLength > 1) ? logLength : 1, '\0');
				glGetShaderInfoLog(shader, logLength, NULL, shaderError.data());

				glGetShaderiv(shader, GL_SHADER_SOURCE_LENGTH, &srcLength);
				std::string shaderSrc((srcLength > 1) ? srcLength : 1, '\0');
				glGetShaderSource(shader, srcLength, NULL, shaderSrc.data());

				std::cout << shaderSrc << std::endl;
				std::cout << (std::format("Error compiling shader: {}", shaderError));
			}
			else
			{
				glGetProgramiv(shader, GL_INFO_LOG_LENGTH, &logLength);
				std::string programError((logLength > 1) ? logLength : 1, '\0');
				glGetProgramInfoLog(shader, logLength, NULL, programError.data());
				std::cout << (std::format("Error linking program: {}", programError));
			}
			return false;
		}
		return true;
	}

	GLuint Program::compileShader(GLenum type, const std::vector<char>& src)
	{
		GLuint shader = glCreateShader(type);
		const char* srcPtr = src.data();
		glShaderSource(shader, 1, &srcPtr, nullptr);
		glCompileShader(shader);
		if (!compileStatus(shader)) return 0;
		return shader;
	}


	void Program::setFloat4(const std::string& name, const glm::vec4& value)
	{
		glUniform4fv(glGetUniformLocation(id, name.c_str()), 1,
			glm::value_ptr(value));
	}

	void Program::setMat4(const std::string& name, const glm::mat4& value)
	{
		glUniformMatrix4fv(glGetUniformLocation(id, name.c_str()), 1,
			GL_FALSE, glm::value_ptr(value));
	}

	void Program::setInt(const std::string& name, int value)
	{
		GLuint uniformLocation = glGetUniformLocation(id, name.c_str());
		glUniform1i(uniformLocation, (GLint)value);
	}
}