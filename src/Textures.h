#ifndef TEXTURES_H
#define TEXTURES_H

#include <string>
#include <unordered_map>
#include <stb/stb_image.h>
#include <iostream>
#include <glad/glad.h>
#include <stdexcept>

class Textures
{
private:
    static std::unordered_map<std::string, GLuint> idMap; // Cache of loaded textures
    static GLuint lastId;                                 // Last bound texture ID

public:
    static GLuint loadTexture(const std::string &path, GLint filter);
    static void bind(GLuint textureId);
};

#endif // TEXTURES_H
