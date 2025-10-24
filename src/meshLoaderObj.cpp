#include "meshLoaderObj.hpp"

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wfloat-equal"
#pragma GCC diagnostic ignored "-Wshadow"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#pragma GCC diagnostic pop

#include <string>
#include <sstream>
#include "meshLoaderObj.hpp"

static float _stringToFloat(const std::string &source) noexcept
{
	std::stringstream ss(source.c_str());
	float result;
	ss >> result;
	return result;
}

[[maybe_unused]]static unsigned int _stringToUint(const std::string &source) noexcept
{
	std::stringstream ss(source.c_str());
	unsigned int result;
	ss >> result;
	return result;
}

static int _stringToInt(const std::string &source) noexcept
{
	std::stringstream ss(source.c_str());
	int result;
	ss >> result;
	return result;
}

static void _stringTokenize(const std::string &source, std::vector<std::string> &tokens) noexcept
{
	tokens.clear();
	std::string aux = source;
	for (unsigned int i = 0; i < aux.size(); i++)
		if (aux[i] == '\t' || aux[i] == '\n')
			aux[i] = ' ';
	std::stringstream ss(aux, std::ios::in);
	while (ss.good())
	{
		std::string s;
		ss >> s;
		if (s.size() > 0)
			tokens.push_back(s);
	}
}

static void _faceTokenize(const std::string &source, std::vector<std::string> &tokens) noexcept
{
	std::string aux = source;
	for (unsigned int i = 0; i < aux.size(); i++)
		if (aux[i] == '\\' || aux[i] == '/')
			aux[i] = ' ';
	_stringTokenize(aux, tokens);
}

Mesh loadObj(const std::string &filename) noexcept
{
	std::vector<Vertex> vertices;
	std::vector<int> indices;

	// Reading Obj file
	std::ifstream file(filename.c_str(), std::ios::in | std::ios::binary);
	if (!file)
	{
		std::cerr << "Model not found " << filename << '\n';
		return {};
	}

	std::string line;
	std::vector<std::string> tokens, facetokens;

	std::vector<glm::vec3> positions;
	positions.reserve(1000);

	std::vector<glm::vec3> normals;
	normals.reserve(1000);

	std::vector<glm::vec2> texcoords;
	texcoords.reserve(1000);

	// Parsing obj file
	while (std::getline(file, line))
	{
		_stringTokenize(line, tokens);

		if (tokens.size() == 0)
			continue;

		// Comments
		if (tokens.size() > 0 && tokens[0].at(0) == '#')
			continue;

		// Vertices
		if (tokens.size() > 3 && tokens[0] == "v")
			positions.push_back(glm::vec3(_stringToFloat(tokens[1]), _stringToFloat(tokens[2]), _stringToFloat(tokens[3])));

		// Normals
		if (tokens.size() > 3 && tokens[0] == "vn")
			normals.push_back(glm::vec3(_stringToFloat(tokens[1]), _stringToFloat(tokens[2]), _stringToFloat(tokens[3])));

		// Texture Coords
		if (tokens.size() > 2 && tokens[0] == "vt")
			texcoords.push_back(glm::vec2(_stringToFloat(tokens[1]), _stringToFloat(tokens[2])));

		// Faces
		if (tokens.size() >= 4 && tokens[0] == "f")
		{
			unsigned int face_format = 0;
			if (tokens[1].find("//") != std::string::npos)
				face_format = 3;
			_faceTokenize(tokens[1], facetokens);

			if (facetokens.size() == 3)
				face_format = 4;
			else
			{
				if (facetokens.size() == 2)
				{
					if (face_format != 3)
						face_format = 2;
				}
				else
				{
					face_format = 1;
				}
			}

			unsigned int index_of_first_vertex_of_face = -1;

			for (unsigned int num_token = 1; num_token < tokens.size(); num_token++)
			{
				if (tokens[num_token].at(0) == '#')
					break;
				_faceTokenize(tokens[num_token], facetokens);

				if (face_format == 1) // Just pos
				{
					int p_index = _stringToInt(facetokens[0]);
					if (p_index > 0)
						p_index -= 1;
					else
						p_index = positions.size() + p_index;

					vertices.push_back(Vertex(positions[p_index].x, positions[p_index].y, positions[p_index].z));
				}
				else if (face_format == 2) // Pos and texcoords
				{
					int p_index = _stringToInt(facetokens[0]);
					if (p_index > 0)
						p_index -= 1;
					else
						p_index = positions.size() + p_index;

					int t_index = _stringToInt(facetokens[1]);
					if (t_index > 0)
						t_index -= 1;
					else
						t_index = texcoords.size() + t_index;

					vertices.push_back(Vertex(positions[p_index].x, positions[p_index].y, positions[p_index].z, texcoords[t_index].x, texcoords[t_index].y));
				}
				else if (face_format == 3)
				{
					// Pos and normal
					int p_index = _stringToInt(facetokens[0]);
					if (p_index > 0)
						p_index -= 1;
					else
						p_index = positions.size() + p_index;

					int n_index = _stringToInt(facetokens[1]);
					if (n_index > 0)
						n_index -= 1;
					else
						n_index = normals.size() + n_index;

					vertices.push_back(Vertex(positions[p_index].x, positions[p_index].y, positions[p_index].z, normals[n_index].x, normals[n_index].y, normals[n_index].z));
				}
				else
				{
					// Normal and texcoord
					int p_index = _stringToInt(facetokens[0]);
					if (p_index > 0)
						p_index -= 1;
					else
						p_index = positions.size() + p_index;

					int t_index = _stringToInt(facetokens[1]);
					if (t_index > 0)
						t_index -= 1;
					else
						t_index = normals.size() + t_index;

					int n_index = _stringToInt(facetokens[2]);
					if (n_index > 0)
						n_index -= 1;
					else
						n_index = normals.size() + n_index;

					vertices.push_back(Vertex(positions[p_index].x, positions[p_index].y, positions[p_index].z, normals[n_index].x, normals[n_index].y, normals[n_index].z, texcoords[t_index].x, texcoords[t_index].y));
				}

				if (num_token < 4)
				{
					if (num_token == 1)
						index_of_first_vertex_of_face = vertices.size() - 1;

					indices.push_back(vertices.size() - 1);
				}
				else
				{
					indices.push_back(index_of_first_vertex_of_face);
					indices.push_back(vertices.size() - 2);
					indices.push_back(vertices.size() - 1);
				}
			}
		}
	}

	std::cout << "Loading:  " << filename << '\n';

	return {vertices, indices};
}

Mesh loadObj(const std::string &filename, std::shared_ptr<std::vector<Texture>> textures) noexcept
{
	Mesh mesh = loadObj(filename);
	mesh.setTextures(textures);

	return mesh;
}