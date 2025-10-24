///@brief
/// Read me [1]

#pragma once

#include <vector>
#include "mesh.hpp"

//Pretty much copy paste, just a function that reads the obj file format, as per doccument specified (no need to create own loader)
Mesh loadObj(const std::string &filename, const std::vector<Texture>& textures) noexcept;
Mesh loadObj(const std::string &filename) noexcept;