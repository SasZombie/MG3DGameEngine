#include "shader.hpp"
#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>

#include <GL/glew.h>
#include <GL/gl.h>
#include <GLFW/glfw3.h>

Shader::Shader(const char *vertexPath, const char *fragmentPath)
	: vertP(vertexPath), fragP(fragmentPath)
{
	std::ifstream vertexShaderFile(vertexPath);
	std::ifstream fragmentShaderFile(fragmentPath);

	if(!vertexShaderFile || !fragmentShaderFile )	
	{
		throw std::runtime_error("Failed to open shader");
	}

	std::stringstream vShaderStream, fShaderStream;

	vShaderStream << vertexShaderFile.rdbuf();
	fShaderStream << fragmentShaderFile.rdbuf();

	vertexShaderFile.close();
	fragmentShaderFile.close();

	std::string vShaderCodeA = vShaderStream.str(); 
	std::string fShaderCodeA = fShaderStream.str(); 
	
	const char *vShaderCode = vShaderCodeA.c_str(); 
	const char *fShaderCode = fShaderCodeA.c_str(); 

	unsigned int vertex, fragment;
	int success;

	vertex = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex, 1, &vShaderCode, NULL);
	glCompileShader(vertex);

	glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		std::cerr << "Error compiling vertex shader! \n";
	}

	fragment = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment, 1, &fShaderCode, NULL);
	glCompileShader(fragment);

	glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		std::cerr << "Error compiling fragment shader! \n";
	}

	GLint Result = GL_FALSE;
	int InfoLogLength;

	glGetShaderiv(vertex, GL_COMPILE_STATUS, &Result);
	glGetShaderiv(vertex, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if (InfoLogLength > 0)
	{
		std::vector<char> VertexShaderErrorMessage(InfoLogLength + 1);
		glGetShaderInfoLog(vertex, InfoLogLength, NULL, &VertexShaderErrorMessage[0]);
		std::cout << VertexShaderErrorMessage[0] << '\n';
	}

	glGetShaderiv(fragment, GL_COMPILE_STATUS, &Result);
	glGetShaderiv(fragment, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if (InfoLogLength > 0)
	{
		std::vector<char> FragmentShaderErrorMessage(InfoLogLength + 1);
		glGetShaderInfoLog(fragment, InfoLogLength, NULL, &FragmentShaderErrorMessage[0]);
		std::cout << FragmentShaderErrorMessage[0] << '\n';
	}

	id = glCreateProgram();
	glAttachShader(id, vertex);
	glAttachShader(id, fragment);
	glLinkProgram(id);

	glGetProgramiv(id, GL_LINK_STATUS, &success);
	if (!success)
	{
		std::cerr << "Error linking shader!\n";
	}

	glDeleteShader(vertex);
	glDeleteShader(fragment);
}

void Shader::use() const noexcept
{
	glUseProgram(id);
}

int Shader::getId() const noexcept
{
	return id;
}

std::ostream &operator<<(std::ostream &os, const Shader &shader)
{
	os << "Shader" << shader.vertP << ' ' << shader.fragP << '\n';

	return os;
}
