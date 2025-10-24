///@brief
///Read me [1]

#pragma once
#include <string>
#include <filesystem>

//Very lightweight class
class Shader
{
public:
	Shader() noexcept = default;
	Shader(const char* vertexPath, const char* fragmentPath);
	~Shader() noexcept = default;
	void use() const noexcept;
	int getId() const noexcept;

    friend std::ostream& operator<<(std::ostream& os, const Shader& shader);


private:
	unsigned int id;

	//These are here for serializing
	std::filesystem::path vertP, fragP;
};

