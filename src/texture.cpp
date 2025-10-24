#include "texture.hpp"
#include <iostream>
#include <GLFW/glfw3.h>
#include <fstream>

GLuint loadBMP(const char *imagepath) noexcept
{

	std::cout << "Reading image " << imagepath << '\n';

	// File header = 14 bytes
	// Info header = 40 bytes
	// Total = 54
	unsigned char header[54];
	unsigned int dataPos;
	unsigned int imageSize;
	unsigned int width, height;

	unsigned char *data;

	std::ifstream file(imagepath, std::ios::binary);

	//No exception
	//If file does not exit, the asset shall be loaded without a texture
	if (!file.is_open())
	{
		std::cout << "Could not open file.\n";
		return 0;
	}

	file.read(reinterpret_cast<char*>(header), 54);
	if (!file)
	{
		std::cout << "Inccorrect BMP file.\n";
		return 0;
	}

	// Parsing BMP file
	if (header[0] != 'B' || header[1] != 'M')
	{
		std::cout << "Not a correct BMP file.\n";
		return 0;
	}

	//Checking for Compression Level
	//I am way too lazy to handle bitCompression
	//So not even gonna check that
	//TODO: Fix aligment issues even if they are not a problem atm
	if (*(int *)&(header[0x1E]) != 0)
	{
		std::cout << "Not a correct BMP file.\n";
		return 0;
	}
	if (*(int *)&(header[0x1C]) != 24)
	{
		std::cout << "Not a correct BMP file.\n";

		return 0;
	}

	dataPos = *(int *)&(header[0x0A]);
	imageSize = *(int *)&(header[0x22]);
	width = *(int *)&(header[0x12]);
	height = *(int *)&(header[0x16]);

	if (imageSize == 0)
		imageSize = width * height * 3;
	if (dataPos == 0)
		dataPos = 54;

	data = new unsigned char[imageSize];

	file.read(reinterpret_cast<char*>(data), imageSize);
	file.close();

	GLuint textureID;
	glGenTextures(1, &textureID);

	glBindTexture(GL_TEXTURE_2D, textureID);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_BGR, GL_UNSIGNED_BYTE, data);

	delete[] data;

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glGenerateMipmap(GL_TEXTURE_2D);

	// Return the ID of the texture
	return textureID;
}

std::vector<Texture> getDefaultTexture(const char *imagepath) noexcept
{
	size_t tex = loadBMP(imagepath);

	std::vector<Texture> textures;
	textures.emplace_back();
	textures[0].id = tex;
	textures[0].type = "texture_diffuse";

	return textures;
}