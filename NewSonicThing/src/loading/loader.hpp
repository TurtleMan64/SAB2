#ifndef LOADER_H
#define LOADER_H

class TexturedModel;

#include <glad/glad.h>
#include <list>
#include <vector>
#include <string>
#include <unordered_map>

#include "../models/models.hpp"

//Loader
class Loader
{
private:
    struct TextureEntry
    {
        GLuint id = GL_NONE;
        int count = 0;
    };

    static int anisotropicFilterIsSupported;
    static float anisotropyLevel;

    static std::list<GLuint> vaos;
    static std::list<GLuint> vbos;
    //static std::list<GLuint> textures;

    static std::unordered_map<std::string, TextureEntry> textures;
    static std::unordered_map<GLuint, std::string> texIdToFilename;

    static int vaoNumber;
    static int vboNumber;
    //static int texNumber;

    static GLuint createVAO();

    static GLuint storeDataInAttributeList(int attributeNumber, int coordinateSize, std::vector<float>* data);

    static void unbindVAO();

    static GLuint bindIndiciesBuffer(std::vector<int>* indicies);

public:
    static void init();

    //For 3D Models
    static RawModel loadToVAO(std::vector<float>* positions, 
                              std::vector<float>* textureCoords, 
                              std::vector<float>* normals, 
                              std::vector<float>* vertexColors, 
                              std::vector<int>* indices);

    //for text
    //returns a std::vector<int> where the first entry is the vao and the rest are vbos
    static std::vector<int> loadToVAO(std::vector<float>* positions, std::vector<float>* textureCoords);

    //for water
    static RawModel loadToVAO(std::vector<float>* positions, int dimensions);

    //for instanced rendering particles
    static GLuint createEmptyVBO(int floatCount);

    static void addInstancedAttribute(GLuint vao, GLuint vbo, int attribute, int dataSize, int instancedDataLength, long long offset);

    static void updateVBO(GLuint vbo, int bufferNumFloats, std::vector<float>* buffer);

    //Loads a texture into GPU memory, returns the GLuint id
    static GLuint loadTexture(const char* filename);

    //Loads a texture without any interpolation
    static GLuint loadTextureNoInterpolation(const char* fileName);

    static void cleanUp();

    static void deleteVAO(GLuint vaoId);

    static void deleteVBO(GLuint vboId);

    static void deleteTexture(GLuint texId);

    static void deleteTexturedModels(std::list<TexturedModel*>* tm);

    static void printInfo();

    static GLuint loadShader(const char* file, int shaderType);
};

#endif
