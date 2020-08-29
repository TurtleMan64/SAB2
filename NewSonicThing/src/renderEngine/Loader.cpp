#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <SOIL/SOIL.h>

#include <vector>  //for std::vector
#include <list>
#include <string>  //for std::string
#include <iostream>
#include <fstream>
#include <cmath>
#include <cstring>

#include "renderEngine.h"
#include "../engineTester/main.h"

#include "../models/models.h"
#include "../toolbox/getline.h"

std::list<GLuint> Loader::vaos;
std::list<GLuint> Loader::vbos;
//std::list<GLuint> Loader::textures;

std::unordered_map<std::string, TextureEntry> Loader::textures;
std::unordered_map<GLuint, std::string> Loader::texIdToFilename;

int Loader::vaoNumber = 0;
int Loader::vboNumber = 0;
//int Loader::texNumber = 0;

RawModel Loader::loadToVAO(std::vector<float>* positions, 
                           std::vector<float>* textureCoords, 
                           std::vector<float>* normals, 
                           std::vector<float>* vertexColors, 
                           std::vector<int>* indicies)
{
    GLuint vaoID = createVAO();
    std::list<GLuint> vboIDs;

    vboIDs.push_back(bindIndiciesBuffer(indicies));
    vboIDs.push_back(storeDataInAttributeList(0, 3, positions));
    vboIDs.push_back(storeDataInAttributeList(1, 2, textureCoords));
    vboIDs.push_back(storeDataInAttributeList(2, 3, normals));
    vboIDs.push_back(storeDataInAttributeList(3, 4, vertexColors));

    unbindVAO();

    return RawModel(vaoID, (int)indicies->size(), &vboIDs);
}

//for text
std::vector<int> Loader::loadToVAO(std::vector<float>* positions, std::vector<float>* textureCoords)
{
    std::vector<int> vertexObjects;

    vertexObjects.push_back(createVAO());
    vertexObjects.push_back(storeDataInAttributeList(0, 2, positions));
    vertexObjects.push_back(storeDataInAttributeList(1, 2, textureCoords));
    unbindVAO();

    return vertexObjects;
}

//for water
RawModel Loader::loadToVAO(std::vector<float>* positions, int dimensions)
{
    GLuint vaoID = createVAO();
    std::list<GLuint> vboIDs;

    vboIDs.push_back(storeDataInAttributeList(0, dimensions, positions));

    unbindVAO();

    return RawModel(vaoID, (int)positions->size() / dimensions, &vboIDs);
}

GLuint Loader::loadTexture(const char* fileName)
{
    if (Loader::textures.find(fileName) != Loader::textures.end())
    {
        //texture is already loaded, return the GLuint
        TextureEntry* entry = &Loader::textures[fileName];
        entry->count = entry->count+1;
        return entry->id;
    }

    int width, height, channels;
    unsigned char* image = SOIL_load_image((Global::pathToEXE + fileName).c_str(), &width, &height, &channels, SOIL_LOAD_RGBA);

    if (image == 0)
    {
        const char* err = SOIL_last_result();
        std::fprintf(stdout, "Error loading image '%s', because '%s'\n", (Global::pathToEXE + fileName).c_str(), err);
        return GL_NONE;
    }

    GLuint textureID = 0;
    glGenTextures(1, &textureID);

    TextureEntry entry;
    entry.count = 1;
    entry.id = textureID;
    textures[fileName] = entry;
    Loader::texIdToFilename[textureID] = fileName;

    glBindTexture(GL_TEXTURE_2D, textureID);

    //Texture wrapping
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    //Texel interpolation
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    //create
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);

    //create mipmap
    glGenerateMipmap(GL_TEXTURE_2D);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_LOD_BIAS, 0.0f); //set to 0 if using anisotropic, around -0.4f if not

    if (glfwExtensionSupported("GL_EXT_texture_filter_anisotropic"))
    {
        //Not sure why these aren't defined... but I've spent too much time trying to fix it.
        const GLuint GL_TEXTURE_MAX_ANISOTROPY_EXT  = 0x84FE;
        const GLuint GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT = 0x84FF;

        float maxAnisotropyLevel;
        glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &maxAnisotropyLevel);
        float amountToUse = fmin(4.0f, maxAnisotropyLevel);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, amountToUse);
    }

    SOIL_free_image_data(image);

    glBindTexture(GL_TEXTURE_2D, 0);
    

    return textureID;
}

GLuint Loader::loadTextureNoInterpolation(const char* fileName)
{
    if (Loader::textures.find(fileName) != Loader::textures.end())
    {
        //texture is already loaded, return the GLuint
        TextureEntry* entry = &Loader::textures[fileName];
        entry->count = entry->count+1;
        return entry->id;
    }

    int width, height, channels;
    unsigned char* image = SOIL_load_image((Global::pathToEXE + fileName).c_str(), &width, &height, &channels, SOIL_LOAD_RGBA);

    if (image == 0)
    {
        const char* err = SOIL_last_result();
        std::fprintf(stdout, "Error loading image '%s', because '%s'\n", (Global::pathToEXE + fileName).c_str(), err);
        return GL_NONE;
    }

    GLuint textureID = 0;
    glGenTextures(1, &textureID);

    TextureEntry entry;
    entry.count = 1;
    entry.id = textureID;
    textures[fileName] = entry;
    Loader::texIdToFilename[textureID] = fileName;

    glBindTexture(GL_TEXTURE_2D, textureID);

    //Texture wrapping
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    //Texel interpolation
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    //create
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);

    SOIL_free_image_data(image);

    glBindTexture(GL_TEXTURE_2D, 0);
    

    return textureID;
}

GLuint Loader::createVAO()
{
    GLuint vaoID = 0;
    glGenVertexArrays(1, &vaoID);
    vaoNumber++;
    vaos.push_back(vaoID);
    glBindVertexArray(vaoID);
    return vaoID;
}

GLuint Loader::storeDataInAttributeList(int attributeNumber, int coordinateSize, std::vector<float>* data)
{
    GLuint vboID = 0;
    glGenBuffers(1, &vboID);
    vboNumber++;
    vbos.push_back(vboID);
    glBindBuffer(GL_ARRAY_BUFFER, vboID);

    glBufferData(GL_ARRAY_BUFFER, data->size()*sizeof(float), (GLvoid*)(&((*data)[0])), GL_STATIC_DRAW); 
    glVertexAttribPointer(attributeNumber, coordinateSize, GL_FLOAT, GL_FALSE, 0, 0); 
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    return vboID;
}

GLuint Loader::bindIndiciesBuffer(std::vector<int>* indicies)
{
    GLuint vboID = 0;
    glGenBuffers(1, &vboID);
    vbos.push_back(vboID);
    vboNumber++;
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vboID);

    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indicies->size() * sizeof(int), (GLvoid*)(&((*indicies)[0])), GL_STATIC_DRAW);

    return vboID;
}

void Loader::unbindVAO()
{
    glBindVertexArray(0);
}

void Loader::cleanUp()
{
    for (auto vaoID : vaos)
    {
        glDeleteVertexArrays(1, &vaoID);
        vaoNumber--;
    }
    vaos.clear();

    for (auto vboID : vbos)
    {
        glDeleteBuffers(1, &vboID);
        vboNumber--;
    }
    vbos.clear();

    std::unordered_map<std::string, TextureEntry>::iterator it = textures.begin();
    while (it != textures.end())
    {
        glDeleteTextures(1, &it->second.id);
        it++;
    }
    textures.clear();
    texIdToFilename.clear();
}

void Loader::deleteVAO(GLuint vaoID)
{
    vaoNumber--;
    glDeleteVertexArrays(1, &vaoID);
    vaos.remove(vaoID);
}

void Loader::deleteVBO(GLuint vboID)
{
    vboNumber--;
    glDeleteBuffers(1, &vboID);
    vbos.remove(vboID);
}

void Loader::deleteTexture(GLuint texID)
{
    std::string filename = texIdToFilename[texID];
    TextureEntry* entry = &textures[filename];
    entry->count = entry->count - 1;

    if (entry->count == 0)
    {
        glDeleteTextures(1, &texID);

        texIdToFilename.erase(texID);
        textures.erase(filename);
    }
}

void Loader::deleteTexturedModels(std::list<TexturedModel*>* tm)
{
    for (auto model : (*tm))
    {
        model->deleteMe();
    }
}

void Loader::printInfo()
{
    std::fprintf(stdout, "VAO Count = %d = %d\n", vaoNumber, (int)vaos.size());
    std::fprintf(stdout, "VBO Count = %d = %d\n", vboNumber, (int)vbos.size());
    //std::fprintf(stdout, "TEX Count = %d = %d\n", texNumber, (int)textures.size());

    //if (textures.size() == 3)
    {
        //for (GLuint i : textures)
        {
            //std::fprintf(stdout, "    %d\n", i);
        }
    }

    std::unordered_map<std::string, TextureEntry>::iterator it = textures.begin();
    while (it != textures.end())
    {
        std::fprintf(stdout, "%s -> %d, %d\n", it->first.c_str(), it->second.id, it->second.count);
        it++;
    }
}

GLuint Loader::loadShader(const char* file, int shaderType)
{
    std::ifstream sourceFile;
    sourceFile.open(Global::pathToEXE+file);
    std::string filetext;

    if (sourceFile.is_open())
    {
        while (!sourceFile.eof())
        {
            std::string line;
            getlineSafe(sourceFile, line);
            filetext.append(line + "\n");
        }

        sourceFile.close();
    }
    else
    {
        std::fprintf(stdout, "Error: Could not find shader file '%s'\n", (Global::pathToEXE + file).c_str());
        sourceFile.close();
        return 0;
    }

    //Go through and replace WINDOW_SIZE
    extern unsigned int SCR_WIDTH;
    extern unsigned int SCR_HEIGHT;
    for (int i = 0; i < (int)filetext.size(); i++)
    {
        if (filetext[i+ 0] == 'W' &&
            filetext[i+ 1] == 'I' &&
            filetext[i+ 2] == 'N' &&
            filetext[i+ 3] == 'D' &&
            filetext[i+ 4] == 'O' &&
            filetext[i+ 5] == 'W' &&
            filetext[i+ 6] == '_' &&
            filetext[i+ 7] == 'S' &&
            filetext[i+ 8] == 'I' &&
            filetext[i+ 9] == 'Z' &&
            filetext[i+10] == 'E' &&
            filetext[i+11] == '.')
        {
            if (filetext[i+12] == 'x')
            {
                memset(&filetext[i], ' ', 13); //replace WINDOW_SIZE.x with all spaces
                std::string windowSizeX = std::to_string(SCR_WIDTH);
                for (int c = 0; c < (int)windowSizeX.size(); c++) //fill in with width of screen
                {
                    filetext[i+c] = windowSizeX[c];
                }
            }
            else if (filetext[i+12] == 'y')
            {
                memset(&filetext[i], ' ', 13); //replace WINDOW_SIZE.y with all spaces
                std::string windowSizeY = std::to_string(SCR_HEIGHT);
                for (int c = 0; c < (int)windowSizeY.size(); c++) //fill in with width of screen
                {
                    filetext[i+c] = windowSizeY[c];
                }
            }
        }
    }

    unsigned int id = glCreateShader(shaderType);
    const char* src = filetext.c_str();
    glShaderSource(id, 1, &src, nullptr);
    glCompileShader(id);

    int result;
    glGetShaderiv(id, GL_COMPILE_STATUS, &result);
    if (result == GL_FALSE)
    {
        int length;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
        char* message = (char*)alloca(length * sizeof(char));
        glGetShaderInfoLog(id, length, &length, message);
        std::cout << "Failed to compile shader!" << std::endl;
        std::cout << message << std::endl;

        glDeleteShader(id);
        return 0;
    }

    return id;
}
