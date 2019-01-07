#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <SOIL/SOIL.h>

#include <vector>  //for std::vector
#include <list>
#include <string>  //for std::string
#include <iostream>
#include <fstream>
#include <cmath>

#include "renderEngine.h"

#include "../models/models.h"

std::list<GLuint> Loader::vaos;
std::list<GLuint> Loader::vbos;
std::list<GLuint> Loader::textures;

int Loader::vaoNumber = 0;
int Loader::vboNumber = 0;
int Loader::texNumber = 0;

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
	vboIDs.push_back(storeDataInAttributeList(3, 3, vertexColors));

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
	GLuint textureID = 0;
	glGenTextures(1, &textureID);
	texNumber++;
	textures.push_back(textureID);

	int width, height, channels;
	unsigned char* image = SOIL_load_image(fileName, &width, &height, &channels, SOIL_LOAD_RGBA);

	if (image == 0)
	{
		const char* err = SOIL_last_result();
		std::fprintf(stdout, "Error loading image '%s', because '%s'\n", fileName, err);
		return 0;
	}

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
	GLuint textureID = 0;
	glGenTextures(1, &textureID);
	texNumber++;
	textures.push_back(textureID);

	int width, height, channels;
	unsigned char* image = SOIL_load_image(fileName, &width, &height, &channels, SOIL_LOAD_RGBA);

	if (image == 0)
	{
		const char* err = SOIL_last_result();
		std::fprintf(stdout, "Error loading image '%s', because '%s'\n", fileName, err);
		return 0;
	}

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

	for (auto texID : textures)
	{
		glDeleteTextures(1, &texID);
		texNumber--;
	}
	textures.clear();
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
	texNumber--;
	glDeleteTextures(1, &texID);
	textures.remove(texID);
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
	std::fprintf(stdout, "TEX Count = %d = %d\n", texNumber, (int)textures.size());

	if (textures.size() == 3)
	{
		for (GLuint i : textures)
		{
			std::fprintf(stdout, "	%d\n", i);
		}
	}
}

GLuint Loader::loadShader(const char* file, int shaderType)
{
	std::ifstream sourceFile;
	sourceFile.open(file);
	std::string filetext;

	if (sourceFile.is_open())
	{
		while (!sourceFile.eof())
		{
			std::string line;
			getline(sourceFile, line);
			filetext.append(line + "\n");
		}

		sourceFile.close();
	}
	else
	{
		std::fprintf(stdout, "Error: Could not find shader file '%s'\n", file);
		sourceFile.close();
		return 0;
	}

	unsigned int id = glCreateShader(shaderType);
	const char* src = filetext.c_str();
	//const int len = filetext.size();
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