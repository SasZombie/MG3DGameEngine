///@brief
/// Read me [1]

#pragma once

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wfloat-equal"
#pragma GCC diagnostic ignored "-Wshadow"

#include <glm/glm.hpp>

#pragma GCC diagnostic pop

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <memory>
#include <vector>
#include <filesystem>
#include "shader.hpp"

struct Vertex
{
	glm::vec3 pos;
	glm::vec3 normals;
	glm::vec2 textureCoords;

	Vertex() noexcept = default;

	Vertex(float pos_x, float pos_y, float pos_z) noexcept
		: pos(pos_x, pos_y, pos_z)
	{
	}

	Vertex(float pos_x, float pos_y, float pos_z, float norm_x, float norm_y, float norm_z) noexcept
		: pos(pos_x, pos_y, pos_z), normals(norm_x, norm_y, norm_z)
	{
	}

	Vertex(float pos_x, float pos_y, float pos_z, float text_x, float text_y) noexcept
		: pos(pos_x, pos_y, pos_z), textureCoords(text_x, text_y)
	{
	}

	Vertex(float pos_x, float pos_y, float pos_z, float norm_x, float norm_y, float norm_z, float text_x, float text_y) noexcept
		: pos(pos_x, pos_y, pos_z), normals(norm_x, norm_y, norm_z), textureCoords(text_x, text_y)
	{
	}
};

struct Texture
{
	unsigned int id;
	std::string type;
	std::filesystem::path path;
};

class Mesh
{
public:
	std::vector<Vertex> vertices;
	std::vector<int> indices;
	std::filesystem::path path;
	std::shared_ptr<std::vector<Texture>> textures;

	unsigned int vao, vbo, ibo;

	Mesh() noexcept = default;
	Mesh(std::vector<Vertex> vertices, std::vector<int> indices, std::shared_ptr<std::vector<Texture>> textures) noexcept;
	Mesh(std::vector<Vertex> vertices, std::vector<int> indices, const std::filesystem::path& pth) noexcept;
	~Mesh() noexcept = default;

	void setTextures(std::shared_ptr<std::vector<Texture>> textures) noexcept;
	void draw(const Shader& shader) noexcept;

	friend std::ostream& operator<<(std::ostream& os, const Mesh& mesh);

private:	
	void setup2() noexcept;
	void setup() noexcept;
};
