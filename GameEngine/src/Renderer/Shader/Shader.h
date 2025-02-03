#pragma once

#include <string>
#include <unordered_map>

#include <glm/glm.hpp>

typedef unsigned int GLenum;
typedef unsigned int GLuint;

namespace GameEngine
{

	class Program
	{
	public:
		Program(const std::string&);
		Program(const std::string&, const std::string&, const std::string&);
		~Program();

		void bind() const;
		void unbind() const;

		void setInt(const std::string&, int);
		void setIntArray(const std::string&, int*, uint32_t);
		void setMat4(const std::string&, const glm::mat4&);
		void setFloat4(const std::string&, const glm::vec4&);
		void setFloat3(const std::string&, const glm::vec3&);
		void setFloat2(const std::string&, const glm::vec2&);
		void setFloat(const std::string&, const float);

		inline const std::string& getName() { return name; }

		//why all these here when they don't seem to be doing anything
		//to imporove the program and seem to just add bloat to the class???

		void uniformInt(const std::string&, const int);
		void unformIntArray(const std::string&, const int*, const uint32_t);
		
		void uniformFloat2(const std::string&, const glm::vec2&);
		void unformFloat3(const std::string&, const glm::vec3&);
		void uniformFloat4(const std::string&, const glm::vec4&);

		void uniformMat3(const std::string&, const glm::mat3&);
		void uniformMat4(const std::string&, const glm::mat4&);

	private:
		std::string readFile(const std::string& filepath);
		std::unordered_map<GLenum, std::string> preProcess(const std::string&);
		void compile(const std::unordered_map<GLenum, std::string>&);

		bool compileStatus(GLuint);
		GLuint compileShader(GLenum, const std::vector<char>&);

		uint32_t id;
		std::string name;
	};
}


