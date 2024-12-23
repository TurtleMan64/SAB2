#include <glad/glad.h>
#include <fstream>
#include <string>
#include <cstring>
#include <iostream>
#include <vector>
#include <unordered_map>
#include <list>

#include "modelloader.hpp"
#include "../models/models.hpp"
#include "../textures/modeltexture.hpp"
#include "../loading/loadergl.hpp"
#include "../toolbox/vector.hpp"
#include "../engineTester/main.hpp"
#include "../toolbox/split.hpp"
#include "../toolbox/getline.hpp"
#include "../collision/collisionmodel.hpp"
#include "../collision/triangle3d.hpp"
#include "../collision/quadtreenode.hpp"
#include "../toolbox/maths.hpp"

ModelLoader::Vertex::Vertex(int index, Vector3f* position, Vector4f* color)
{
    this->index = index;
    this->color.set(color);
    this->position.set(position->x, position->y, position->z);
    this->length = position->length();
    this->textureIndex = NO_INDEX;
    this->normalIndex = NO_INDEX;
    this->duplicateVertex = nullptr;
}

ModelLoader::Vertex::Vertex(int index, Vector3f* position)
{
    this->index = index;
    //this->color.set(Maths::nextUniform(), Maths::nextUniform(), Maths::nextUniform());
    this->color.set(1.0f, 1.0f, 1.0f, 1.0f);
    this->position.set(position->x, position->y, position->z);
    this->length = position->length();
    this->textureIndex = NO_INDEX;
    this->normalIndex = NO_INDEX;
    this->duplicateVertex = nullptr;
}

int ModelLoader::Vertex::isSet()
{
    if ((textureIndex != NO_INDEX) && (normalIndex != NO_INDEX))
    {
        return 1;
    }
    return 0;
}

int ModelLoader::Vertex::hasSameTextureAndNormal(int textureIndexOther, int normalIndexOther)
{
    if ((textureIndexOther == textureIndex) && (normalIndexOther == normalIndex))
    {
        return 1;
    }
    return 0;
}

ModelLoader::FakeTexture::FakeTexture()
{
    name = "";
    type = 0;
    sound = -1;
    particle = 0;
}

int ModelLoader::loadModel(std::vector<TexturedModel*>* models, std::string filePath, std::string fileName)
{
    int attemptBinaryOBJ = loadBinaryModel(models, filePath, fileName+".binobj");
    
    if (attemptBinaryOBJ == -1)
    {
        int attemptVCL = loadVclModel(models, filePath, fileName+".binvcl");

        if (attemptVCL == -1)
        {
            int attemptOBJ = loadObjModel(models, filePath, fileName+".obj");

            if (attemptOBJ == -1)
            {
                std::fprintf(stderr, "Error: Cannot load file '%s' or '%s'\n", 
                    ((filePath + fileName) + ".bin").c_str(), 
                    ((filePath + fileName) + ".obj").c_str());
            }

            return attemptOBJ;
        }

        return attemptVCL;
    }

    return attemptBinaryOBJ;
}

//int numAdditionalVertices = 0;

//Each TexturedModel contained within 'models' must be deleted later.
int ModelLoader::loadBinaryModel(std::vector<TexturedModel*>* models, std::string filePath, std::string fileName)
{
    if (models->size() > 0)
    {
        return 1;
    }

    FILE* file = nullptr;

    #ifdef _WIN32
    int err = fopen_s(&file, (Global::pathToEXE+filePath+fileName).c_str(), "rb");
    if (file == nullptr || err != 0)
    {
        return -1;
    }
    #else
    file = fopen((Global::pathToEXE+filePath+fileName).c_str(), "rb");
    if (file == nullptr)
    {
        return -1;
    }
    #endif

    char fileType[4];
    ModelLoader::read(fileType, 4, file);
    if (fileType[0] != 'o' || 
        fileType[1] != 'b' ||
        fileType[2] != 'j' ||
        fileType[3] != 0)
    {
        printf("Error: File '%s' is not a valid .binobj file\n", (filePath+fileName).c_str());
        return -2;
    }

    std::string line;

    std::string mtlname = "";
    std::vector<Vertex*>  vertices;
    std::vector<Vector2f> textures;
    std::vector<Vector3f> normals;
    std::vector<RawModel> rawModelsList;
    std::vector<ModelTexture> modelTextures;

    int mtllibLength;
    ModelLoader::read(&mtllibLength, 4, file);
    for (int i = 0; i < mtllibLength; i++)
    {
        char nextChar;
        ModelLoader::read(&nextChar, 1, file);
        mtlname = mtlname + nextChar;
    }

    std::unordered_map<std::string, ModelTexture> mtlMap;
    parseMtl(filePath, mtlname, &mtlMap);


    int numVertices;
    ModelLoader::read(&numVertices, 4, file);
    vertices.reserve(numVertices*2);
    for (int i = 0; i < numVertices; i++)
    {
        float t[3];
        ModelLoader::read(t, 12, file);

        Vector3f vertex(t[0], t[1], t[2]);
        Vertex* newVertex = new Vertex((int)vertices.size(), &vertex); INCR_NEW("Vertex");
        vertices.push_back(newVertex);
    }

    int numTexCoords;
    ModelLoader::read(&numTexCoords, 4, file);
    textures.reserve(numTexCoords);
    for (int i = 0; i < numTexCoords; i++)
    {
        float t[2];
        ModelLoader::read(t, 8, file);

        Vector2f texCoord(t[0], t[1]);
        textures.push_back(texCoord);
    }

    int numNormals;
    ModelLoader::read(&numNormals, 4, file);
    normals.reserve(numNormals);
    for (int i = 0; i < numNormals; i++)
    {
        float t[3];
        ModelLoader::read(t, 12, file);

        Vector3f normal(t[0], t[1], t[2]);
        normals.push_back(normal);
    }

    //int bytesPerIndV;
    //fread(&bytesPerIndV, sizeof(int), 1, file);
    //int bytesPerIndVT;
    //fread(&bytesPerIndVT, sizeof(int), 1, file);
    //int bytesPerIndVN;
    //fread(&bytesPerIndVN, sizeof(int), 1, file);

    int numMaterials;
    ModelLoader::read(&numMaterials, 4, file);
    rawModelsList.reserve(numMaterials);
    for (int m = 0; m < numMaterials; m++)
    {
        int matnameLength;
        ModelLoader::read(&matnameLength, 4, file);
        std::string matname = "";
        for (int c = 0; c < matnameLength; c++)
        {
            char nextChar;
            ModelLoader::read(&nextChar, 1, file);
            matname = matname + nextChar;
        }

        modelTextures.push_back(mtlMap[matname]);

        std::vector<int> indices;
        int numFaces;
        ModelLoader::read(&numFaces, 4, file);
        indices.reserve(numFaces*9);
        for (int i = 0; i < numFaces; i++)
        {
            //int f[9] = {0,0,0,0,0,0,0,0,0};
            //
            //fread(&f[0], bytesPerIndV,  1, file);
            //fread(&f[1], bytesPerIndVT, 1, file);
            //fread(&f[2], bytesPerIndVN, 1, file);
            //fread(&f[3], bytesPerIndV,  1, file);
            //fread(&f[4], bytesPerIndVT, 1, file);
            //fread(&f[5], bytesPerIndVN, 1, file);
            //fread(&f[6], bytesPerIndV,  1, file);
            //fread(&f[7], bytesPerIndVT, 1, file);
            //fread(&f[8], bytesPerIndVN, 1, file);

            int f[9];

            ModelLoader::read(&f[0], 36, file);

            processVertexBinary(f[0], f[1], f[2], &vertices, &indices);
            processVertexBinary(f[3], f[4], f[5], &vertices, &indices);
            processVertexBinary(f[6], f[7], f[8], &vertices, &indices);
        }

        //save the model we've been building so far...
        removeUnusedVertices(&vertices);

        std::vector<float> verticesArray;
        std::vector<float> texturesArray;
        std::vector<float> normalsArray;
        std::vector<float> colorsArray;
        convertDataToArrays(&vertices, &textures, &normals, &verticesArray, &texturesArray, &normalsArray, &colorsArray);
        rawModelsList.push_back(LoaderGL::loadToVAO(&verticesArray, &texturesArray, &normalsArray, &colorsArray, &indices));
    }

    fclose(file);

    //go through rawModelsList and modelTextures to construct and add to the given TexturedModel list
    for (unsigned int i = 0; i < rawModelsList.size(); i++)
    {
        TexturedModel* tm = new TexturedModel(&rawModelsList[i], &modelTextures[i]); INCR_NEW("TexturedModel");
        models->push_back(tm);
    }

    for (auto vertex : vertices)
    {
        delete vertex; INCR_DEL("Vertex");
    }

    deleteUnusedMtl(&mtlMap, &modelTextures);
    modelTextures.clear();
    mtlMap.clear();

    return 0;
}

//Each TexturedModel contained within 'models' must be deleted later.
int ModelLoader::loadVclModel(std::vector<TexturedModel*>* models, std::string filePath, std::string fileName)
{
    if (models->size() > 0)
    {
        return 1;
    }

    FILE* file = nullptr;

    #ifdef _WIN32
    int err = fopen_s(&file, (Global::pathToEXE+filePath+fileName).c_str(), "rb");
    if (file == nullptr || err != 0)
    {
        return -1;
    }
    #else
    file = fopen((Global::pathToEXE+filePath+fileName).c_str(), "rb");
    if (file == nullptr)
    {
        return -1;
    }
    #endif

    char fileType[4];
    ModelLoader::read(fileType, 4, file);
    if (fileType[0] != 'v' || 
        fileType[1] != 'c' ||
        fileType[2] != 'l')
    {
        printf("Error: File '%s' is not a valid .binvcl file\n", (filePath+fileName).c_str());
        return -2;
    }

    int vclVersion = (int)fileType[3];

    std::string line;

    std::string mtlname = "";
    std::vector<Vertex*>  vertices;
    std::vector<Vector2f> textures;
    std::vector<Vector3f> normals;
    //float* textures = nullptr;
    //float* normals = nullptr;
    std::vector<RawModel> rawModelsList;
    std::vector<ModelTexture> modelTextures;

    int mtllibLength;
    ModelLoader::read(&mtllibLength, 4, file);
    for (int i = 0; i < mtllibLength; i++)
    {
        char nextChar;
        ModelLoader::read(&nextChar, 1, file);
        mtlname = mtlname + nextChar;
    }

    std::unordered_map<std::string, ModelTexture> mtlMap;
    parseMtl(filePath, mtlname, &mtlMap);

    int numVertices;
    ModelLoader::read(&numVertices, 4, file);
    vertices.reserve(numVertices*2 + 1);
    for (int i = 0; i < numVertices; i++)
    {
        float t[3];
        ModelLoader::read(t, 12, file);

        Vector3f vertex(t[0], t[1], t[2]);
        Vertex* newVertex = new Vertex((int)vertices.size(), &vertex); INCR_NEW("Vertex");

        unsigned char c[3];
        ModelLoader::read(c, 3, file);
        float red   = ((float)c[0])/255.0f;
        float green = ((float)c[1])/255.0f;
        float blue  = ((float)c[2])/255.0f;
        float alpha = 1.0f;
        if (c[0] == 153 && c[1] == 102) //special case, use b as alpha
        {
            alpha = blue;
            red   = 1.0f;
            green = 1.0f;
            blue  = 1.0f;
        }
        newVertex->color.set(red, green, blue, alpha); //no vertex alpha in VCOL file...
        vertices.push_back(newVertex);
    }

    //printf("%d regular vertices\n", numVertices);
    //numAdditionalVertices = 0;

    int numTexCoords;
    ModelLoader::read(&numTexCoords, 4, file);

    //textures = new float[numTexCoords*2];

    //ModelLoader::read(textures, numTexCoords*8, file);

    textures.reserve(numTexCoords + 1);
    for (int i = 0; i < numTexCoords; i++)
    {
        float t[2];
        ModelLoader::read(t, 8, file);
    
        Vector2f texCoord(t[0], t[1]);
        textures.push_back(texCoord);
    }

    //int bytesPerIndV;
    //fread(&bytesPerIndV, sizeof(int), 1, file);
    //int bytesPerIndVT;
    //fread(&bytesPerIndVT, sizeof(int), 1, file);

    int numNormals = 1;

    if (vclVersion == 0)
    {
        Vector3f normal(0, 1, 0); //hard coded normal of up
        normals.push_back(normal);

        //normals = new float[3];
        //normals[0] = 0.0f;
        //normals[1] = 1.0f;
        //normals[2] = 0.0f;
    }
    else
    {
        ModelLoader::read(&numNormals, 4, file);
        //normals = new float[numNormals*3];

        //for (int i = 0; i < numNormals; i++)
        //{
        //    char vn[3];
        //    ModelLoader::read(vn, 3, file);
        //
        //    Vector3f norm(vn[0], vn[1], vn[2]);
        //    norm.normalize();
        //    normals[i*3 + 0] = norm.x;
        //    normals[i*3 + 1] = norm.y;
        //    normals[i*3 + 2] = norm.z;
        //}

        normals.reserve(numNormals + 1);
        for (int i = 0; i < numNormals; i++)
        {
            char vn[3];
            ModelLoader::read(vn, 3, file);
        
            Vector3f norm(vn[0], vn[1], vn[2]);
            norm.normalize();
            normals.push_back(norm);
        }
    }

    int numMaterials;
    ModelLoader::read(&numMaterials, 4, file);
    rawModelsList.reserve(numMaterials);
    modelTextures.reserve(numMaterials + 1);
    for (int m = 0; m < numMaterials; m++)
    {
        int matnameLength;
        ModelLoader::read(&matnameLength, 4, file);
        std::string matname = "";
        for (int c = 0; c < matnameLength; c++)
        {
            char nextChar;
            ModelLoader::read(&nextChar, 1, file);
            matname = matname + nextChar;
        }

        modelTextures.push_back(mtlMap[matname]);

        std::vector<int> indices;
        int numFaces;
        ModelLoader::read(&numFaces, 4, file);
        indices.reserve(numFaces*9 + 1);
        for (int i = 0; i < numFaces; i++)
        {
            //int f[6] = {0,0,0,0,0,0};
            //
            //fread(&f[0], bytesPerIndV,  1, file);
            //fread(&f[1], bytesPerIndVT, 1, file);
            //fread(&f[2], bytesPerIndV,  1, file);
            //fread(&f[3], bytesPerIndVT, 1, file);
            //fread(&f[4], bytesPerIndV,  1, file);
            //fread(&f[5], bytesPerIndVT, 1, file);

            if (vclVersion == 0)
            {
                int f[6];

                ModelLoader::read(f, 24, file);

                processVertexBinary(f[0], f[1], 1, &vertices, &indices);
                processVertexBinary(f[2], f[3], 1, &vertices, &indices);
                processVertexBinary(f[4], f[5], 1, &vertices, &indices);
            }
            else
            {
                int f[9];

                ModelLoader::read(f, 36, file);

                processVertexBinary(f[0], f[1], f[2], &vertices, &indices);
                processVertexBinary(f[3], f[4], f[5], &vertices, &indices);
                processVertexBinary(f[6], f[7], f[8], &vertices, &indices);
            }
        }

        //save the model we've been building so far...
        removeUnusedVertices(&vertices);

        std::vector<float> verticesArray;
        std::vector<float> texturesArray;
        std::vector<float> normalsArray;
        std::vector<float> colorsArray;
        convertDataToArrays(&vertices, &textures, &normals, &verticesArray, &texturesArray, &normalsArray, &colorsArray);
        rawModelsList.push_back(LoaderGL::loadToVAO(&verticesArray, &texturesArray, &normalsArray, &colorsArray, &indices));
    }

    //printf("%d new vertices\n", numAdditionalVertices);

    fclose(file);

    //go through rawModelsList and modelTextures to construct and add to the given TexturedModel list
    for (unsigned int i = 0; i < rawModelsList.size(); i++)
    {
        TexturedModel* tm = new TexturedModel(&rawModelsList[i], &modelTextures[i]); INCR_NEW("TexturedModel");
        models->push_back(tm);
    }

    for (auto vertex : vertices)
    {
        delete vertex; INCR_DEL("Vertex");
    }

    deleteUnusedMtl(&mtlMap, &modelTextures);
    modelTextures.clear();
    mtlMap.clear();

    return 0;
}

int ModelLoader::loadObjModel(std::vector<TexturedModel*>* models, std::string filePath, std::string fileName)
{
    if (models->size() > 0)
    {
        return 1;
    }

    std::ifstream file(Global::pathToEXE+filePath+fileName);
    if (!file.is_open())
    {
        //printf("Error: Cannot load file '%s'\n", (filePath + fileName).c_str());
        file.close();
        return -1;
    }

    std::string line;

    std::vector<Vertex*> vertices;
    std::vector<Vector2f> textures;
    std::vector<Vector3f> normals;
    std::vector<int> indices;

    std::unordered_map<std::string, ModelTexture> mtlMap;

    std::vector<RawModel> rawModelsList;
    std::vector<ModelTexture> modelTextures;

    int foundFaces = 0;


    //clock_t t;
    //t = clock();
    //printf("Calculating total...\n");

    while (!file.eof())
    {
        getlineSafe(file, line);
        //printf("line = '%s'\n", line.c_str());

        char lineBuf[256];
        memcpy(lineBuf, line.c_str(), line.size()+1);

        int splitLength = 0;
        char** lineSplit = split(lineBuf, ' ', &splitLength);

        if (splitLength > 0)
        {
            if (foundFaces == 0)
            {
                //Find the mtl filename
                if (strcmp(lineSplit[0], "mtllib") == 0)
                {
                    parseMtl(filePath, lineSplit[1], &mtlMap);
                }
                else if (strcmp(lineSplit[0], "v") == 0)
                {
                    std::string p1(lineSplit[1]);
                    std::string p2(lineSplit[2]);
                    std::string p3(lineSplit[3]);
                    Vector3f position(std::stof(p1, nullptr), std::stof(p2, nullptr), std::stof(p3, nullptr));

                    if (splitLength == 7) //with RGB vertex colors
                    {
                        std::string c1(lineSplit[4]);
                        std::string c2(lineSplit[5]);
                        std::string c3(lineSplit[6]);

                        float r = std::stof(c1, nullptr);
                        float g = std::stof(c2, nullptr);
                        float b;
                        float a;
                        if (r == 0.6f && g == 0.4f) //special case, use b as alpha
                        {
                            r = 1.0f;
                            g = 1.0f;
                            b = 1.0f;
                            a = std::stof(c3, nullptr);
                        }
                        else
                        {
                            b = std::stof(c3, nullptr);
                            a = 1.0f;
                        }
                        Vector4f colors(r, g, b, a);

                        Vertex* newVertex = new Vertex((int)vertices.size(), &position, &colors); INCR_NEW("Vertex");
                        vertices.push_back(newVertex);
                    }
                    else if (splitLength == 8) //with RGBA vertex colors
                    {
                        float r = std::stof(lineSplit[4], nullptr);
                        float g = std::stof(lineSplit[5], nullptr);
                        float b = std::stof(lineSplit[6], nullptr);
                        float a = std::stof(lineSplit[7], nullptr);
                        Vector4f colors(r, g, b, a);

                        Vertex* newVertex = new Vertex((int)vertices.size(), &position, &colors); INCR_NEW("Vertex");
                        vertices.push_back(newVertex);
                    }
                    else
                    {
                        Vertex* newVertex = new Vertex((int)vertices.size(), &position); INCR_NEW("Vertex");
                        vertices.push_back(newVertex);
                    }
                }
                else if (strcmp(lineSplit[0], "vt") == 0)
                {
                    std::string t1(lineSplit[1]);
                    std::string t2(lineSplit[2]);
                    Vector2f texCoord(std::stof(t1, nullptr), std::stof(t2, nullptr));
                    textures.push_back(texCoord);
                }
                else if (strcmp(lineSplit[0], "vn") == 0)
                {
                    std::string n1(lineSplit[1]);
                    std::string n2(lineSplit[2]);
                    std::string n3(lineSplit[3]);
                    Vector3f normal(std::stof(n1, nullptr), std::stof(n2, nullptr), std::stof(n3, nullptr));
                    normals.push_back(normal);
                }
                else if (strcmp(lineSplit[0], "usemtl") == 0) //first usetml found, before any faces entered
                {
                    auto material = mtlMap.find(lineSplit[1]);
                    if (material != mtlMap.end())
                    {
                        modelTextures.push_back(material->second);
                    }
                    else
                    {
                        std::string fn = Global::pathToEXE + filePath + fileName;
                        printf("Error: Trying to use material '%s', but it doesn't exist. (%s)\n", lineSplit[1], fn.c_str());
                    }
                }
                else if (strcmp(lineSplit[0], "f") == 0)
                {
                    foundFaces = 1;
                }
            }

            if (foundFaces == 1)
            {
                if (strcmp(lineSplit[0], "f") == 0)
                {
                    int dummy = 0;
                    char** vertex1 = split(lineSplit[1], '/', &dummy);
                    char** vertex2 = split(lineSplit[2], '/', &dummy);
                    char** vertex3 = split(lineSplit[3], '/', &dummy);

                    processVertex(vertex1, &vertices, &indices);
                    processVertex(vertex2, &vertices, &indices);
                    processVertex(vertex3, &vertices, &indices);

                    free(vertex1);
                    free(vertex2);
                    free(vertex3);
                }
                else if (strcmp(lineSplit[0], "usemtl") == 0 && (vertices.size() > 0)) //found another new material, so save the previous model and start a new one
                {
                    auto material = mtlMap.find(lineSplit[1]);
                    if (material != mtlMap.end())
                    {
                        modelTextures.push_back(material->second);
                    }
                    else
                    {
                        std::string fn = Global::pathToEXE + filePath + fileName;
                        printf("Error: Trying to use material '%s', but it doesn't exist. (%s)\n", lineSplit[1], fn.c_str());
                    }

                    //save the model we've been building so far...
                    removeUnusedVertices(&vertices);
                    std::vector<float> verticesArray;
                    std::vector<float> texturesArray;
                    std::vector<float> normalsArray;
                    std::vector<float> colorsArray;

                    convertDataToArrays(&vertices, &textures, &normals, &verticesArray, &texturesArray, &normalsArray, &colorsArray);
                    rawModelsList.push_back(LoaderGL::loadToVAO(&verticesArray, &texturesArray, &normalsArray, &colorsArray, &indices));

                    indices.clear();
                }
            }
        }
        free(lineSplit);
    }
    file.close();

    //t = clock() - t;
    //printf("Total: It took me %d clicks (%f seconds).\n", t, ((float)t) / CLOCKS_PER_SEC);

    removeUnusedVertices(&vertices);

    std::vector<float> verticesArray;
    std::vector<float> texturesArray;
    std::vector<float> normalsArray;
    std::vector<float> colorsArray;
    convertDataToArrays(&vertices, &textures, &normals, &verticesArray, &texturesArray, &normalsArray, &colorsArray);
    rawModelsList.push_back(LoaderGL::loadToVAO(&verticesArray, &texturesArray, &normalsArray, &colorsArray, &indices));

    //go through rawModelsList and modelTextures to construct and add to the given TexturedModel list
    for (unsigned int i = 0; i < rawModelsList.size(); i++)
    {
        TexturedModel* tm = new TexturedModel(&rawModelsList[i], &modelTextures[i]); INCR_NEW("TexturedModel");
        models->push_back(tm);
    }

    for (auto vertex : vertices)
    {
        delete vertex; INCR_DEL("Vertex");
    }

    deleteUnusedMtl(&mtlMap, &modelTextures);
    modelTextures.clear();
    mtlMap.clear();

    return 0;
}

void ModelLoader::parseMtl(std::string filePath, std::string fileName, std::unordered_map<std::string, ModelTexture>* outMtlMap)
{
    //map that we fill in, from the mtl file
    if (!outMtlMap->empty())
    {
        std::fprintf(stderr, "Warning: Material map is not empty.\n");
    }
    outMtlMap->clear();

    std::ifstream file(Global::pathToEXE+filePath+fileName);
    if (!file.is_open())
    {
        std::fprintf(stderr, "Error: Cannot load file '%s'\n", (Global::pathToEXE + filePath + fileName).c_str());
        file.close();
        return;
    }

    //clock_t t;
    //t = clock();
    //printf("Calculating MTL...\n");

    std::string line;

    //default values
    std::string currentMaterialName = "DefaultMtl";
    float currentShineDamperValue = 20.0f;
    float currentReflectivityValue = 0.0f;
    float currentTransparencyValue = 1.0f;
    float currentFakeLightingValue = 1.0f;
    float currentGlowAmountValue = 0.0f;
    float currentScrollXValue = 0.0f;
    float currentScrollYValue = 0.0f;
    int   currentNumImages = 1;
    float currentAnimSpeed = 0.0f;
    int   currentMixingType = 1;
    float currentFogScale = 1.0f;
    int   currentRenderOrder = 0;

    while (!file.eof())
    {
        getlineSafe(file, line);

        char lineBuf[256];
        memcpy(lineBuf, line.c_str(), line.size()+1);

        int splitLength = 0;
        char** lineSplit = split(lineBuf, ' ', &splitLength);

        if (splitLength > 0)
        {
            if (strcmp(lineSplit[0], "newmtl") == 0) //new material found, add its name to array
            {
                currentMaterialName = lineSplit[1];
                currentShineDamperValue = 0.0f;
                currentReflectivityValue = 0.0f;
                currentTransparencyValue = 1.0f;
                currentFakeLightingValue = 1.0f;
                currentGlowAmountValue = 0.0f;
                currentScrollXValue = 0.0f;
                currentScrollYValue = 0.0f;
                currentNumImages = 1;
                currentAnimSpeed = 0.0f;
                currentMixingType = 1;
                currentFogScale = 1.0f;
                currentRenderOrder = 0;
            }
            else if (strcmp(lineSplit[0], "\tmap_Kd") == 0 || strcmp(lineSplit[0], "map_Kd") == 0) //end of material found, generate it with all its attrributes
            {
                std::string imageFilenameString = filePath+lineSplit[1];
                char* fname = (char*)imageFilenameString.c_str();

                std::vector<GLuint> textureIds;
                textureIds.push_back(LoaderGL::loadTexture(fname)); //generate new texture

                currentNumImages--;
                while (currentNumImages > 0)
                {
                    free(lineSplit);

                    getlineSafe(file, line);

                    memcpy(lineBuf, line.c_str(), line.size()+1);

                    lineSplit = split(lineBuf, ' ', &splitLength);

                    char* nextFilename = lineSplit[0];

                    if (lineSplit[0][0] == '\t')
                    {
                        nextFilename = &lineSplit[0][1];
                    }

                    imageFilenameString = filePath+nextFilename;
                    fname = (char*)imageFilenameString.c_str();
                    textureIds.push_back(LoaderGL::loadTexture(fname)); //load the new texture

                    currentNumImages--;
                }

                std::vector<GLuint>* texturesPointer = &textureIds;
                ModelTexture newTexture(texturesPointer);

                newTexture.shineDamper = currentShineDamperValue;
                newTexture.reflectivity = currentReflectivityValue;
                newTexture.hasTransparency = true;
                newTexture.useFakeLighting = false;
                if (currentTransparencyValue > 0.0f)
                {
                    newTexture.hasTransparency = false;
                }
                if (currentFakeLightingValue < 1.0f)
                {
                    newTexture.useFakeLighting = true;
                }
                newTexture.glowAmount = currentGlowAmountValue;
                newTexture.scrollX = currentScrollXValue;
                newTexture.scrollY = currentScrollYValue;
                newTexture.animationSpeed = currentAnimSpeed;
                newTexture.mixingType = currentMixingType;
                newTexture.fogScale = currentFogScale;
                newTexture.renderOrder = (char)currentRenderOrder;

                (*outMtlMap)[currentMaterialName] = newTexture; //put a copy of newTexture into the list
            }
            else if (strcmp(lineSplit[0], "\tNs") == 0 || strcmp(lineSplit[0], "Ns") == 0)
            {
                currentShineDamperValue = std::stof(lineSplit[1]);
            }
            else if (strcmp(lineSplit[0], "\tNi") == 0 || strcmp(lineSplit[0], "Ni") == 0)
            {
                currentReflectivityValue = std::stof(lineSplit[1]);
            }
            else if (strcmp(lineSplit[0], "\tTr") == 0 || strcmp(lineSplit[0], "Tr") == 0)
            {
                currentTransparencyValue = std::stof(lineSplit[1]);
            }
            else if (strcmp(lineSplit[0], "\td") == 0 || strcmp(lineSplit[0], "d") == 0)
            {
                currentFakeLightingValue = std::stof(lineSplit[1]);
            }
            else if (strcmp(lineSplit[0], "\tglow") == 0 || strcmp(lineSplit[0], "glow") == 0)
            {
                currentGlowAmountValue = std::stof(lineSplit[1]);
            }
            else if (strcmp(lineSplit[0], "\tscrollX") == 0 || strcmp(lineSplit[0], "scrollX") == 0)
            {
                currentScrollXValue = std::stof(lineSplit[1]);
            }
            else if (strcmp(lineSplit[0], "\tscrollY") == 0 || strcmp(lineSplit[0], "scrollY") == 0)
            {
                currentScrollYValue = std::stof(lineSplit[1]);
            }
            else if (strcmp(lineSplit[0], "\tanimSpeed") == 0 || strcmp(lineSplit[0], "animSpeed") == 0)
            {
                currentAnimSpeed = std::stof(lineSplit[1]);
                if (currentAnimSpeed < 0)
                {
                    std::fprintf(stderr, "Error: animSpeed was negative.\n");
                    currentAnimSpeed = 0;
                }
            }
            else if (strcmp(lineSplit[0], "\tnumImages") == 0 || strcmp(lineSplit[0], "numImages") == 0)
            {
                currentNumImages = std::stoi(lineSplit[1]);
                if (currentNumImages < 1)
                {
                    std::fprintf(stderr, "Error: numImages was negative.\n");
                    currentNumImages = 1;
                }
            }
            else if (strcmp(lineSplit[0], "\tmixLinear") == 0 || strcmp(lineSplit[0], "mixLinear") == 0)
            {
                currentMixingType = 2;
            }
            else if (strcmp(lineSplit[0], "\tmixSinusoidal") == 0 || strcmp(lineSplit[0], "mixSinusoidal") == 0)
            {
                currentMixingType = 3;
            }
            else if (strcmp(lineSplit[0], "\tfogScale") == 0 || strcmp(lineSplit[0], "fogScale") == 0)
            {
                currentFogScale = std::stof(lineSplit[1]);
            }
            else if (strcmp(lineSplit[0], "\trenderOrder") == 0 || strcmp(lineSplit[0], "renderOrder") == 0)
            {
                currentRenderOrder = Maths::clamp(0, std::stoi(lineSplit[1]), 5);
            }
        }

        free(lineSplit);
    }
    file.close();

    //t = clock() - t;
    //printf("MTL: It took me %d clicks (%f seconds).\n", t, ((float)t) / CLOCKS_PER_SEC);

}

int ModelLoader::loadObjModelWithMTL(std::vector<TexturedModel*>* models, std::string filePath, std::string fileNameOBJ, std::string fileNameMTL)
{
    if (models->size() > 0)
    {
        return 1;
    }

    std::ifstream file(Global::pathToEXE + filePath + fileNameOBJ);
    if (!file.is_open())
    {
        std::fprintf(stderr, "Error: Cannot load file '%s'\n", (Global::pathToEXE + filePath + fileNameOBJ).c_str());
        file.close();
        return -1;
    }

    std::string line;

    std::vector<Vertex*> vertices;
    std::vector<Vector2f> textures;
    std::vector<Vector3f> normals;
    std::vector<int> indices;

    std::vector<RawModel> rawModelsList;
    std::vector<ModelTexture> modelTextures;

    int foundFaces = 0;

    std::unordered_map<std::string, ModelTexture> mtlMap;
    parseMtl(filePath, fileNameMTL, &mtlMap);

    while (!file.eof())
    {
        getlineSafe(file, line);

        char lineBuf[256];
        memcpy(lineBuf, line.c_str(), line.size()+1);

        int splitLength = 0;
        char** lineSplit = split(lineBuf, ' ', &splitLength);

        if (splitLength > 0)
        {
            if (foundFaces == 0)
            {
                if (strcmp(lineSplit[0], "v") == 0)
                {
                    std::string p1(lineSplit[1]);
                    std::string p2(lineSplit[2]);
                    std::string p3(lineSplit[3]);
                    Vector3f position(std::stof(p1, nullptr), std::stof(p2, nullptr), std::stof(p3, nullptr));

                    if (splitLength == 7) //with RGB vertex colors
                    {
                        std::string c1(lineSplit[4]);
                        std::string c2(lineSplit[5]);
                        std::string c3(lineSplit[6]);
                        
                        float r = std::stof(c1, nullptr);
                        float g = std::stof(c2, nullptr);
                        float b;
                        float a;
                        if (r == 0.6f && g == 0.4f) //special case, use b as alpha
                        {
                            r = 1.0f;
                            g = 1.0f;
                            b = 1.0f;
                            a = std::stof(c3, nullptr);
                        }
                        else
                        {
                            b = std::stof(c3, nullptr);
                            a = 1.0f;
                        }
                        Vector4f colors(r, g, b, a);

                        Vertex* newVertex = new Vertex((int)vertices.size(), &position, &colors); INCR_NEW("Vertex");
                        vertices.push_back(newVertex);
                    }
                    else if (splitLength == 8) //with RGBA vertex colors
                    {
                        float r = std::stof(lineSplit[4], nullptr);
                        float g = std::stof(lineSplit[5], nullptr);
                        float b = std::stof(lineSplit[6], nullptr);
                        float a = std::stof(lineSplit[7], nullptr);
                        Vector4f colors(r, g, b, a);

                        Vertex* newVertex = new Vertex((int)vertices.size(), &position, &colors); INCR_NEW("Vertex");
                        vertices.push_back(newVertex);
                    }
                    else
                    {
                        Vertex* newVertex = new Vertex((int)vertices.size(), &position); INCR_NEW("Vertex");
                        vertices.push_back(newVertex);
                    }
                }
                else if (strcmp(lineSplit[0], "vt") == 0)
                {
                    std::string t1(lineSplit[1]);
                    std::string t2(lineSplit[2]);
                    Vector2f texCoord(std::stof(t1, nullptr), std::stof(t2, nullptr));
                    textures.push_back(texCoord);
                }
                else if (strcmp(lineSplit[0], "vn") == 0)
                {
                    std::string n1(lineSplit[1]);
                    std::string n2(lineSplit[2]);
                    std::string n3(lineSplit[3]);
                    Vector3f normal(std::stof(n1, nullptr), std::stof(n2, nullptr), std::stof(n3, nullptr));
                    normals.push_back(normal);
                }
                else if (strcmp(lineSplit[0], "usemtl") == 0) //first usetml found, before any faces entered
                {
                    auto material = mtlMap.find(lineSplit[1]);
                    if (material != mtlMap.end())
                    {
                        modelTextures.push_back(material->second);
                    }
                    else
                    {
                        std::string fn = Global::pathToEXE + filePath + fileNameOBJ;
                        printf("Error: Trying to use material '%s', but it doesn't exist. (%s)\n", lineSplit[1], fn.c_str());
                    }
                }
                else if (strcmp(lineSplit[0], "f") == 0)
                {
                    foundFaces = 1;
                }
            }

            if (foundFaces == 1)
            {
                if (strcmp(lineSplit[0], "f") == 0)
                {
                    int dummy = 0;
                    char** vertex1 = split(lineSplit[1], '/', &dummy);
                    char** vertex2 = split(lineSplit[2], '/', &dummy);
                    char** vertex3 = split(lineSplit[3], '/', &dummy);

                    processVertex(vertex1, &vertices, &indices);
                    processVertex(vertex2, &vertices, &indices);
                    processVertex(vertex3, &vertices, &indices);

                    free(vertex1);
                    free(vertex2);
                    free(vertex3);
                }
                else if (strcmp(lineSplit[0], "usemtl") == 0 && (vertices.size() > 0)) //found another new material, so save the previous model and start a new one
                {
                    auto material = mtlMap.find(lineSplit[1]);
                    if (material != mtlMap.end())
                    {
                        modelTextures.push_back(material->second);
                    }
                    else
                    {
                        std::string fn = Global::pathToEXE + filePath + fileNameOBJ;
                        printf("Error: Trying to use material '%s', but it doesn't exist. (%s)\n", lineSplit[1], fn.c_str());
                    }

                    //save the model we've been building so far...
                    removeUnusedVertices(&vertices);
                    std::vector<float> verticesArray;
                    std::vector<float> texturesArray;
                    std::vector<float> normalsArray;
                    std::vector<float> colorsArray;
                    convertDataToArrays(&vertices, &textures, &normals, &verticesArray, &texturesArray, &normalsArray, &colorsArray);
                    rawModelsList.push_back(LoaderGL::loadToVAO(&verticesArray, &texturesArray, &normalsArray, &colorsArray, &indices));

                    indices.clear();
                }
            }
        }
        free(lineSplit);
    }
    file.close();

    removeUnusedVertices(&vertices);
    std::vector<float> verticesArray;
    std::vector<float> texturesArray;
    std::vector<float> normalsArray;
    std::vector<float> colorsArray;
    convertDataToArrays(&vertices, &textures, &normals, &verticesArray, &texturesArray, &normalsArray, &colorsArray);
    rawModelsList.push_back(LoaderGL::loadToVAO(&verticesArray, &texturesArray, &normalsArray, &colorsArray, &indices)); //put a copy of the final model into rawModelsList

    //go through rawModelsList and modelTextures to construct and add to the given TexturedModel list
    for (unsigned int i = 0; i < rawModelsList.size(); i++)
    {
        TexturedModel* tm = new TexturedModel(&rawModelsList[i], &modelTextures[i]); INCR_NEW("TexturedModel");
        models->push_back(tm);
    }

    for (auto vertex : vertices)
    {
        delete vertex; INCR_DEL("Vertex");
    }

    deleteUnusedMtl(&mtlMap, &modelTextures);
    modelTextures.clear();
    mtlMap.clear();

    return 0;
}

int ModelLoader::loadBinaryModelWithMTL(std::vector<TexturedModel*>* models, std::string filePath, std::string fileNameBin, std::string fileNameMTL)
{
    if (models->size() > 0)
    {
        return 1;
    }

    FILE* file = nullptr;

    #ifdef _WIN32
    int err = fopen_s(&file, (Global::pathToEXE+filePath+fileNameBin).c_str(), "rb");
    if (file == nullptr || err != 0)
    {
        printf("Error: Cannot load file '%s'\n", (filePath + fileNameBin).c_str());
        return -1;
    }
    #else
    file = fopen((Global::pathToEXE+filePath+fileNameBin).c_str(), "rb");
    if (file == nullptr)
    {
        printf("Error: Cannot load file '%s'\n", (filePath + fileNameBin).c_str());
        return -1;
    }
    #endif

    char fileType[4];
    ModelLoader::read(fileType, 4, file);
    if (fileType[0] != 'o' || 
        fileType[1] != 'b' ||
        fileType[2] != 'j' ||
        fileType[3] != 0)
    {
        printf("Error: File '%s' is not a valid .binobj file\n", (filePath+fileNameBin).c_str());
        return -2;
    }

    std::string line;

    std::string mtlname = "";
    std::vector<Vertex*>  vertices;
    std::vector<Vector2f> textures;
    std::vector<Vector3f> normals;
    std::vector<RawModel> rawModelsList;
    std::vector<ModelTexture> modelTextures;

    int mtllibLength;
    ModelLoader::read(&mtllibLength, 4, file);
    for (int i = 0; i < mtllibLength; i++)
    {
        char nextChar;
        ModelLoader::read(&nextChar, 1, file);
        mtlname = mtlname + nextChar;
    }

    std::unordered_map<std::string, ModelTexture> mtlMap;
    parseMtl(filePath, fileNameMTL, &mtlMap);

    int numVertices;
    ModelLoader::read(&numVertices, 4, file);
    vertices.reserve(numVertices);
    for (int i = 0; i < numVertices; i++)
    {
        float t[3];
        ModelLoader::read(t, 12, file);

        Vector3f vertex(t[0], t[1], t[2]);
        Vertex* newVertex = new Vertex((int)vertices.size(), &vertex); INCR_NEW("Vertex");
        vertices.push_back(newVertex);
    }

    int numTexCoords;
    ModelLoader::read(&numTexCoords, 4, file);
    textures.reserve(numTexCoords);
    for (int i = 0; i < numTexCoords; i++)
    {
        float t[2];
        ModelLoader::read(t, 8, file);

        Vector2f texCoord(t[0], t[1]);
        textures.push_back(texCoord);
    }

    int numNormals;
    ModelLoader::read(&numNormals, 4, file);
    normals.reserve(numNormals);
    for (int i = 0; i < numNormals; i++)
    {
        float t[3];
        ModelLoader::read(t, 12, file);

        Vector3f normal(t[0], t[1], t[2]);
        normals.push_back(normal);
    }

    //int bytesPerIndV;
    //fread(&bytesPerIndV, sizeof(int), 1, file);
    //int bytesPerIndVT;
    //fread(&bytesPerIndVT, sizeof(int), 1, file);
    //int bytesPerIndVN;
    //fread(&bytesPerIndVN, sizeof(int), 1, file);

    int numMaterials;
    ModelLoader::read(&numMaterials, 4, file);
    rawModelsList.reserve(numMaterials);
    for (int m = 0; m < numMaterials; m++)
    {
        int matnameLength;
        ModelLoader::read(&matnameLength, 4, file);
        std::string matname = "";
        for (int c = 0; c < matnameLength; c++)
        {
            char nextChar;
            ModelLoader::read(&nextChar, 1, file);
            matname = matname + nextChar;
        }

        modelTextures.push_back(mtlMap[matname]);

        std::vector<int> indices;
        int numFaces;
        ModelLoader::read(&numFaces, 4, file);
        indices.reserve(numFaces*9);
        for (int i = 0; i < numFaces; i++)
        {
            //int f[9] = {0,0,0,0,0,0,0,0,0};
            //
            //fread(&f[0], bytesPerIndV,  1, file);
            //fread(&f[1], bytesPerIndVT, 1, file);
            //fread(&f[2], bytesPerIndVN, 1, file);
            //fread(&f[3], bytesPerIndV,  1, file);
            //fread(&f[4], bytesPerIndVT, 1, file);
            //fread(&f[5], bytesPerIndVN, 1, file);
            //fread(&f[6], bytesPerIndV,  1, file);
            //fread(&f[7], bytesPerIndVT, 1, file);
            //fread(&f[8], bytesPerIndVN, 1, file);

            int f[9];

            ModelLoader::read(&f[0], 36, file);

            processVertexBinary(f[0], f[1], f[2], &vertices, &indices);
            processVertexBinary(f[3], f[4], f[5], &vertices, &indices);
            processVertexBinary(f[6], f[7], f[8], &vertices, &indices);
        }

        //save the model we've been building so far...
        removeUnusedVertices(&vertices);

        std::vector<float> verticesArray;
        std::vector<float> texturesArray;
        std::vector<float> normalsArray;
        std::vector<float> colorsArray;
        convertDataToArrays(&vertices, &textures, &normals, &verticesArray, &texturesArray, &normalsArray, &colorsArray);
        rawModelsList.push_back(LoaderGL::loadToVAO(&verticesArray, &texturesArray, &normalsArray, &colorsArray, &indices));
    }

    fclose(file);

    //go through rawModelsList and modelTextures to construct and add to the given TexturedModel list
    for (unsigned int i = 0; i < rawModelsList.size(); i++)
    {
        TexturedModel* tm = new TexturedModel(&rawModelsList[i], &modelTextures[i]); INCR_NEW("TexturedModel");
        models->push_back(tm);
    }

    for (auto vertex : vertices)
    {
        delete vertex; INCR_DEL("Vertex");
    }

    deleteUnusedMtl(&mtlMap, &modelTextures);
    modelTextures.clear();
    mtlMap.clear();

    return 0;
}

void ModelLoader::processVertex(char** vertex,
    std::vector<Vertex*>* vertices,
    std::vector<int>* indices)
{
    int index = atoi(vertex[0]) - 1;
    int textureIndex = atoi(vertex[1]) - 1;
    int normalIndex = atoi(vertex[2]) - 1;

    Vertex* currentVertex = (*vertices)[index]; //check bounds on this?
    if (currentVertex->isSet() == 0)
    {
        currentVertex->textureIndex = textureIndex;
        currentVertex->normalIndex = normalIndex;
        indices->push_back(index);
    }
    else
    {
        dealWithAlreadyProcessedVertex(currentVertex, textureIndex, normalIndex, indices, vertices);
    }
}

void ModelLoader::processVertexBinary(int vIndex, int tIndex, int nIndex,
    std::vector<Vertex*>* vertices,
    std::vector<int>* indices)
{
    vIndex--;
    tIndex--;
    nIndex--;

    Vertex* currentVertex = (*vertices)[vIndex]; //check bounds on this?
    if (currentVertex->isSet() == 0)
    {
        currentVertex->textureIndex = tIndex;
        currentVertex->normalIndex = nIndex;
        indices->push_back(vIndex);
    }
    else
    {
        dealWithAlreadyProcessedVertex(currentVertex, tIndex, nIndex, indices, vertices);
    }
}

void ModelLoader::dealWithAlreadyProcessedVertex(
    Vertex* previousVertex,
    int newTextureIndex,
    int newNormalIndex,
    std::vector<int>* indices,
    std::vector<Vertex*>* vertices)
{
    if (previousVertex->hasSameTextureAndNormal(newTextureIndex, newNormalIndex))
    {
        indices->push_back(previousVertex->index);
    }
    else
    {
        Vertex* anotherVertex = previousVertex->duplicateVertex;
        if (anotherVertex != nullptr)
        {
            dealWithAlreadyProcessedVertex(anotherVertex, newTextureIndex, newNormalIndex, indices, vertices);
        }
        else
        {
            Vertex* duplicateVertex = new Vertex((int)vertices->size(), &previousVertex->position, &previousVertex->color); INCR_NEW("Vertex");
            //numAdditionalVertices++;
            duplicateVertex->textureIndex = newTextureIndex;
            duplicateVertex->normalIndex = newNormalIndex;

            previousVertex->duplicateVertex = duplicateVertex;
            vertices->push_back(duplicateVertex);
            indices->push_back(duplicateVertex->index);
        }
    }
}

void ModelLoader::convertDataToArrays(
    std::vector<Vertex*>* vertices, 
    std::vector<Vector2f>* textures,
    std::vector<Vector3f>* normals, 
    std::vector<float>* verticesArray, 
    std::vector<float>* texturesArray,
    std::vector<float>* normalsArray,
    std::vector<float>* colorsArray)
{
    // TODO dont dereference these
    for (auto currentVertex : (*vertices))
    {
        Vector3f* position = &currentVertex->position;
        Vector2f* textureCoord = &(*textures)[currentVertex->textureIndex];
        Vector3f* normalVector = &(*normals)[currentVertex->normalIndex];
        verticesArray->push_back(position->x);
        verticesArray->push_back(position->y);
        verticesArray->push_back(position->z);
        texturesArray->push_back(textureCoord->x);
        texturesArray->push_back(1 - textureCoord->y);
        normalsArray->push_back(normalVector->x);
        normalsArray->push_back(normalVector->y);
        normalsArray->push_back(normalVector->z);
        colorsArray->push_back(currentVertex->color.x);
        colorsArray->push_back(currentVertex->color.y);
        colorsArray->push_back(currentVertex->color.z);
        colorsArray->push_back(currentVertex->color.w);
    }
}

void ModelLoader::removeUnusedVertices(std::vector<Vertex*>* vertices)
{
    const int numVertices = (int)vertices->size();
    for (int i = 0; i < numVertices; i++)
    {
        Vertex* vertex = vertices->at(i);
        if (vertex->isSet() == 0)
        {
            vertex->textureIndex = 0;
            vertex->normalIndex = 0;
        }
    }
}

CollisionModel* ModelLoader::loadCollisionModel(std::string filePath, std::string fileName)
{
    CollisionModel* collisionModel = new CollisionModel; INCR_NEW("CollisionModel");

    std::string objFileName = Global::pathToEXE + "res/" + filePath + fileName + ".obj";
    std::string mtlFileName = "default.mtl";

    std::list<FakeTexture> fakeTextures;

    char currType = 0;
    char currSound = 0;
    char currParticle = 0;

    std::ifstream file(objFileName);
    if (!file.is_open())
    {
        printf("Error: Cannot load file '%s'\n", objFileName.c_str());
        file.close();
        return collisionModel;
    }

    std::string line;

    std::vector<Vector3f> vertices;

    while (!file.eof())
    {
        getlineSafe(file, line);

        char lineBuf[256];
        memcpy(lineBuf, line.c_str(), line.size()+1);

        int splitLength = 0;
        char** lineSplit = split(lineBuf, ' ', &splitLength);

        if (splitLength > 0)
        {
            if (strcmp(lineSplit[0], "v") == 0)
            {
                Vector3f vertex;
                vertex.x = std::stof(lineSplit[1]);
                vertex.y = std::stof(lineSplit[2]);
                vertex.z = std::stof(lineSplit[3]);
                vertices.push_back(vertex);
            }
            else if (strcmp(lineSplit[0], "f") == 0)
            {
                int len = 0;
                char** vertex1 = split(lineSplit[1], '/', &len);
                char** vertex2 = split(lineSplit[2], '/', &len);
                char** vertex3 = split(lineSplit[3], '/', &len);

                Vector3f* vert1 = &vertices[std::stoi(vertex1[0]) - 1];
                Vector3f* vert2 = &vertices[std::stoi(vertex2[0]) - 1];
                Vector3f* vert3 = &vertices[std::stoi(vertex3[0]) - 1];

                Triangle3D* tri = new Triangle3D(vert1, vert2, vert3, currType, currSound, currParticle); INCR_NEW("Triangle3D");

                collisionModel->triangles.push_back(tri);

                free(vertex1);
                free(vertex2);
                free(vertex3);
            }
            else if (strcmp(lineSplit[0], "usemtl") == 0)
            {
                currType = 0;
                currSound = -1;
                currParticle = 0;

                bool found = false;

                for (FakeTexture dummy : fakeTextures)
                {
                    if (dummy.name == lineSplit[1])
                    {
                        currType = dummy.type;
                        currSound = dummy.sound;
                        currParticle = dummy.particle;
                        found = true;
                    }
                }

                if (!found)
                {
                    printf("Error: Trying to use collision material '%s', but it doesn't exist. (%s) (%s)\n", lineSplit[1], objFileName.c_str(), mtlFileName.c_str());
                }
            }
            else if (strcmp(lineSplit[0], "mtllib") == 0)
            {
                mtlFileName = Global::pathToEXE + "res/" + filePath + lineSplit[1];
                std::ifstream fileMTL(mtlFileName);
                if (!fileMTL.is_open())
                {
                    printf("Error: Cannot load file '%s'\n", mtlFileName.c_str());
                    fileMTL.close();
                    file.close();
                    return collisionModel;
                }

                std::string lineMTL;

                while (!fileMTL.eof())
                {
                    getlineSafe(fileMTL, lineMTL);

                    char lineBufMTL[256];
                    memcpy(lineBufMTL, lineMTL.c_str(), lineMTL.size()+1);

                    int splitLengthMTL = 0;
                    char** lineSplitMTL = split(lineBufMTL, ' ', &splitLengthMTL);

                    if (splitLengthMTL > 1)
                    {
                        if (strcmp(lineSplitMTL[0], "newmtl") == 0)
                        {
                            FakeTexture fktex;

                            fktex.name = lineSplitMTL[1];
                            fakeTextures.push_back(fktex);
                        }
                        else if (strcmp(lineSplitMTL[0], "type") == 0 ||
                                 strcmp(lineSplitMTL[0], "\ttype") == 0)
                        {
                            if (strcmp(lineSplitMTL[1], "dig") == 0)
                            {
                                fakeTextures.back().type |= 1;
                            }
                            else if (strcmp(lineSplitMTL[1], "wall") == 0)
                            {
                                fakeTextures.back().type |= 2;
                            }
                            else if (strcmp(lineSplitMTL[1], "death") == 0)
                            {
                                fakeTextures.back().type |= 4;
                            }
                            else if (strcmp(lineSplitMTL[1], "bounce") == 0)
                            {
                                fakeTextures.back().type |= 8;

                                float bouncePower = std::fminf(std::stof(lineSplitMTL[2]), 700.0f);
                        
                                //bounce speed only has resolution of 100
                                fakeTextures.back().type |= (((char)round(bouncePower/100.0f)) << 5);
                            }
                            else if (strcmp(lineSplitMTL[1], "nocam") == 0)
                            {
                                fakeTextures.back().type |= 16;
                            }
                        }
                        else if (strcmp(lineSplitMTL[0], "sound") == 0 ||
                                 strcmp(lineSplitMTL[0], "\tsound") == 0)
                        {
                            fakeTextures.back().sound = (char)round(std::stof(lineSplitMTL[1]));
                        }
                        else if (strcmp(lineSplitMTL[0], "particle") == 0 ||
                                 strcmp(lineSplitMTL[0], "\tparticle") == 0)
                        {
                            fakeTextures.back().particle = (char)round(std::stof(lineSplitMTL[1]));
                        }
                    }
                    free(lineSplitMTL);
                }
                fileMTL.close();
            }
        }
        free(lineSplit);
    }
    file.close();

    collisionModel->generateMinMaxValues();

    return collisionModel;
}

void ModelLoader::workOnQuadTreeNode(FILE* file, QuadTreeNode* node)
{
    //read in header for this node
    ModelLoader::read(&node->depth,          4, file);
    ModelLoader::read(&node->xMid,           4, file);
    ModelLoader::read(&node->zMid,           4, file);
    ModelLoader::read(&node->xMinHorizontal, 4, file);
    ModelLoader::read(&node->xMaxHorizontal, 4, file);
    ModelLoader::read(&node->yMinHorizontal, 4, file);
    ModelLoader::read(&node->yMaxHorizontal, 4, file);
    ModelLoader::read(&node->zMinHorizontal, 4, file);
    ModelLoader::read(&node->zMaxHorizontal, 4, file);
    int numTriangles;
    ModelLoader::read(&numTriangles,         4, file);
    node->trisHorizontal.reserve(numTriangles+1); //not sure if i need to +1 but might as well

    //read in all the triangles
    for (int i = 0; i < numTriangles; i++)
    {
        char triBuf[92];
        ModelLoader::read(triBuf, 92, file);
        Triangle3D* newTri = new Triangle3D(triBuf); INCR_NEW("Triangle3D");
        node->trisHorizontal.push_back(newTri);
    }

    char children[4];
    ModelLoader::read(children, 4, file);

    if (children[0] != 0) //top left
    {
        QuadTreeNode* topLeftChild = new QuadTreeNode; INCR_NEW("QuadTreeNode");
        workOnQuadTreeNode(file, topLeftChild);
        node->topLeft = topLeftChild;
    }

    if (children[1] != 0) //top right
    {
        QuadTreeNode* topRightChild = new QuadTreeNode; INCR_NEW("QuadTreeNode");
        workOnQuadTreeNode(file, topRightChild);
        node->topRight = topRightChild;
    }

    if (children[2] != 0) //bot left
    {
        QuadTreeNode* botLeftChild = new QuadTreeNode; INCR_NEW("QuadTreeNode");
        workOnQuadTreeNode(file, botLeftChild);
        node->botLeft = botLeftChild;
    }

    if (children[3] != 0) //bot right
    {
        QuadTreeNode* botRightChild = new QuadTreeNode; INCR_NEW("QuadTreeNode");
        workOnQuadTreeNode(file, botRightChild);
        node->botRight = botRightChild;
    }
}

inline void ModelLoader::read(void* buf, int numBytesToRead, FILE* file)
{
    int numRead = (int)fread(buf, sizeof(char), numBytesToRead, file);
    if (numRead != numBytesToRead)
    {
        printf("Warning: Read only %d bytes instead of %d\n", numRead, numBytesToRead);
    }
}

CollisionModel* ModelLoader::loadBinaryQuadTree(std::string filePath, std::string fileName)
{
    FILE* file = nullptr;

    #ifdef _WIN32
    int err = fopen_s(&file, (Global::pathToEXE + "res/" + filePath+fileName+".qtree").c_str(), "rb");
    if (file == nullptr || err != 0)
    {
        printf("Error: Cannot load file '%s'\n", (Global::pathToEXE + "res/" + filePath+fileName+".qtree").c_str());
        return nullptr;
    }
    #else
    file = fopen((Global::pathToEXE + "res/" + filePath+fileName+".qtree").c_str(), "rb");
    if (file == nullptr)
    {
        printf("Error: Cannot load file '%s'\n", (Global::pathToEXE + "res/" + filePath+fileName+".qtree").c_str());
        return nullptr;
    }
    #endif

    CollisionModel* collisionModel = new CollisionModel; INCR_NEW("CollisionModel");

    char fileType[4];
    ModelLoader::read(fileType, 4, file);
    if (fileType[0] != 'q' ||
        fileType[1] != 't' ||
        fileType[2] != 'r' ||
        fileType[3] != 'e')
    {
        printf("Error: File '%s' is not a valid .qtree file\n", (Global::pathToEXE + "res/" + filePath+fileName+".qtree").c_str());
        return collisionModel;
    }

    //collision model header
    ModelLoader::read(&collisionModel->treeMaxDepth,   4, file);
    ModelLoader::read(&collisionModel->leafNodeWidth,  4, file);
    ModelLoader::read(&collisionModel->leafNodeHeight, 4, file);
    ModelLoader::read(&collisionModel->maxX,           4, file);
    ModelLoader::read(&collisionModel->minX,           4, file);
    ModelLoader::read(&collisionModel->maxY,           4, file);
    ModelLoader::read(&collisionModel->minY,           4, file);
    ModelLoader::read(&collisionModel->maxZ,           4, file);
    ModelLoader::read(&collisionModel->minZ,           4, file);
    int hasRootNode;
    ModelLoader::read(&hasRootNode,                    4, file);

    //read rest of file recursively in helper function
    if (hasRootNode)
    {
        QuadTreeNode* root = new QuadTreeNode; INCR_NEW("QuadTreeNode");
        workOnQuadTreeNode(file, root);
        collisionModel->quadTreeRoot = root;
    }

    fclose(file);

    return collisionModel;
}

CollisionModel* ModelLoader::loadBinaryCollisionModel(std::string filePath, std::string fileName)
{
    std::list<FakeTexture> fakeTextures;
    std::vector<Vector3f> vertices;

    char currType = 0;
    char currSound = 0;
    char currParticle = 0;

    FILE* file = nullptr;

    #ifdef _WIN32
    int err = fopen_s(&file, (Global::pathToEXE + "res/" + filePath+fileName+".bincol").c_str(), "rb");
    if (file == nullptr || err != 0)
    {
        printf("Error: Cannot load file '%s'\n", (Global::pathToEXE + "res/" + filePath+fileName+".bincol").c_str());
        return nullptr;
    }
    #else
    file = fopen((Global::pathToEXE + "res/" + filePath+fileName+".bincol").c_str(), "rb");
    if (file == nullptr)
    {
        printf("Error: Cannot load file '%s'\n", (Global::pathToEXE + "res/" + filePath+fileName+".bincol").c_str());
        return nullptr;
    }
    #endif

    CollisionModel* collisionModel = new CollisionModel; INCR_NEW("CollisionModel");

    char fileType[4];
    ModelLoader::read(fileType, 4, file);
    if (fileType[0] != 'c' || 
        fileType[1] != 'o' ||
        fileType[2] != 'l' ||
        fileType[3] != 0)
    {
        printf("Error: File '%s' is not a valid .bincol file\n", (Global::pathToEXE + "res/" + filePath+fileName+".bincol").c_str());
        return collisionModel;
    }

    std::string mtlname = "";
    int mtllibLength;
    ModelLoader::read(&mtllibLength, 4, file);
    for (int i = 0; i < mtllibLength; i++)
    {
        char nextChar;
        ModelLoader::read(&nextChar, 1, file);
        mtlname = mtlname + nextChar;
    }

    {
        std::ifstream fileMTL(Global::pathToEXE + "res/" + filePath + mtlname);
        if (!fileMTL.is_open())
        {
            printf("Error: Cannot load file '%s'\n", (Global::pathToEXE + "res/" + filePath + mtlname).c_str());
            fileMTL.close();
            fclose(file);
            return collisionModel;
        }

        std::string lineMTL;

        while (!fileMTL.eof())
        {
            getlineSafe(fileMTL, lineMTL);

            char lineBufMTL[256];
            memcpy(lineBufMTL, lineMTL.c_str(), lineMTL.size()+1);

            int splitLengthMTL = 0;
            char** lineSplitMTL = split(lineBufMTL, ' ', &splitLengthMTL);

            if (splitLengthMTL > 1)
            {
                if (strcmp(lineSplitMTL[0], "newmtl") == 0)
                {
                    FakeTexture fktex;

                    fktex.name = lineSplitMTL[1];
                    fakeTextures.push_back(fktex);
                }
                else if (strcmp(lineSplitMTL[0], "type") == 0 ||
                         strcmp(lineSplitMTL[0], "\ttype") == 0)
                {
                    if (strcmp(lineSplitMTL[1], "dig") == 0)
                    {
                        fakeTextures.back().type |= 1;
                    }
                    else if (strcmp(lineSplitMTL[1], "wall") == 0)
                    {
                        fakeTextures.back().type |= 2;
                    }
                    else if (strcmp(lineSplitMTL[1], "death") == 0)
                    {
                        fakeTextures.back().type |= 4;
                    }
                    else if (strcmp(lineSplitMTL[1], "bounce") == 0)
                    {
                        fakeTextures.back().type |= 8;

                        float bouncePower = std::fminf(std::stof(lineSplitMTL[2]), 700.0f);
                        
                        //bounce speed only has resolution of 100
                        fakeTextures.back().type |= (((char)round(bouncePower/100.0f)) << 5);
                    }
                    else if (strcmp(lineSplitMTL[1], "nocam") == 0)
                    {
                        fakeTextures.back().type |= 16;
                    }
                }
                else if (strcmp(lineSplitMTL[0], "sound") == 0 ||
                         strcmp(lineSplitMTL[0], "\tsound") == 0)
                {
                    fakeTextures.back().sound = (char)round(std::stof(lineSplitMTL[1]));
                }
                else if (strcmp(lineSplitMTL[0], "particle") == 0 ||
                         strcmp(lineSplitMTL[0], "\tparticle") == 0)
                {
                    fakeTextures.back().particle = (char)round(std::stof(lineSplitMTL[1]));
                }
            }
            free(lineSplitMTL);
        }
        fileMTL.close();
    }


    int numVertices;
    ModelLoader::read(&numVertices, 4, file);
    vertices.reserve(numVertices);
    for (int i = 0; i < numVertices; i++)
    {
        float t[3];
        ModelLoader::read(t, 12, file);

        Vector3f vertex(t[0], t[1], t[2]);
        vertices.push_back(vertex);
    }

    //int bytesPerIndV;
    //fread(&bytesPerIndV, sizeof(int), 1, file);

    int numMaterials;
    ModelLoader::read(&numMaterials, 4, file);
    for (int m = 0; m < numMaterials; m++)
    {
        int matnameLength;
        ModelLoader::read(&matnameLength, 4, file);
        std::string matname = "";
        for (int c = 0; c < matnameLength; c++)
        {
            char nextChar;
            ModelLoader::read(&nextChar, 1, file);
            matname = matname + nextChar;
        }

        currType = 0;
        currSound = -1;
        currParticle = 0;

        for (FakeTexture dummy : fakeTextures)
        {
            if (dummy.name == matname)
            {
                currType = dummy.type;
                currSound = dummy.sound;
                currParticle = dummy.particle;
            }
        }

        std::vector<int> indices;
        int numFaces;
        ModelLoader::read(&numFaces, 4, file);
        indices.reserve(numFaces*3);
        for (int i = 0; i < numFaces; i++)
        {
            //int f[3] = {0,0,0};
            //
            //fread(&f[0], bytesPerIndV, 1, file);
            //fread(&f[1], bytesPerIndV, 1, file);
            //fread(&f[2], bytesPerIndV, 1, file);

            int f[3];

            ModelLoader::read(&f[0], 12, file);

            Triangle3D* tri = new Triangle3D(&vertices[f[0]-1], &vertices[f[1]-1], &vertices[f[2]-1], currType, currSound, currParticle); INCR_NEW("Triangle3D");

            collisionModel->triangles.push_back(tri);
        }
    }
    fclose(file);

    collisionModel->generateMinMaxValues();

    return collisionModel;
}

void ModelLoader::deleteUnusedMtl(std::unordered_map<std::string, ModelTexture>* mtlMap, std::vector<ModelTexture>* usedMtls)
{
    //for some extremely bizarre reason, I cannot make a set or a map on the stack in this specific function.
    // "trying to use deleted function" or something...
    //so we must do a slow O(n^2) operation here instead of linear...
    //printf("mtlMapSize = %d, usedMtls size = %d\n", mtlMap->size(), usedMtls->size());
    //iterate through loaded mtls, check if they are used
    std::unordered_map<std::string, ModelTexture>::iterator it;
    for (it = mtlMap->begin(); it != mtlMap->end(); it++)
    {
        bool found = false;
        for (int i = 0; i < (int)usedMtls->size(); i++)
        {
            ModelTexture* texUsed = &(usedMtls->at(i));
            ModelTexture* texInBigMap = &(it->second);
            if (texUsed->equalTo(texInBigMap))
            {
                found = true;
                break;
            }
        }

        // No one is using this material, so let's delete it.
        if (!found)
        {
            it->second.deleteMe();
        }
    }
}
