#pragma once
#include <string>
#include <unordered_map>
#include <stb/stb_image.h>
#include <iostream>

#include <stdexcept>

typedef unsigned int GLuint;
typedef signed int GLint;

class Textures
{
private:
    static std::unordered_map<std::string, GLuint> idMap; 
    static GLuint lastId;                      

public:
    static GLuint loadTexture(const std::string &path, GLint filter);
    static void bind(GLuint textureId);
};
