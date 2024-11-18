#include "Textures.h"

std::unordered_map<std::string, GLuint> Textures::idMap;
GLuint Textures::lastId = 0;

GLuint Textures::loadTexture(const std::string &path, GLint filter)
{
    if (idMap.find(path) != idMap.end())
    {
        return idMap[path];
    }

    int width, height, numChannels;
    unsigned char *data = stbi_load(path.c_str(), &width, &height, &numChannels, 4); 

    if (!data)
    {
        throw std::runtime_error("Failed to load texture: " + path);
    }

    GLuint textureId;
    glGenTextures(1, &textureId);
    bind(textureId);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filter);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filter);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);

    idMap[path] = textureId;
    stbi_image_free(data);

    return textureId;
}

void Textures::bind(GLuint textureId)
{
    if (textureId != lastId)
    {
        glBindTexture(GL_TEXTURE_2D, textureId);
        lastId = textureId;
    }
}
