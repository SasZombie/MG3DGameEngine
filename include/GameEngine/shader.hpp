///@brief
///Read me [1]

#pragma once

//Very lightweight class
class Shader
{
public:
	Shader() noexcept = default;
	Shader(const char* vertexPath, const char* fragmentPath);
	~Shader() noexcept = default;
	void use() const noexcept;
	int getId() const noexcept;

private:
	unsigned int id;
};

