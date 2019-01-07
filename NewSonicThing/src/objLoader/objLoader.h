#ifndef OBJLOADER_H
#define OBJLOADER_H

class TexturedModel;
class CollisionModel;

#include <list>
#include <string>

//Attempts to load a mode as either an OBJ or binary format.
//Checks for binary file first, then tries OBJ.
//Each TexturedModel contained within 'models' must be deleted later.
//Returns 0 if successful, 1 if model is already loaded, -1 if file couldn't be loaded
int loadModel(std::list<TexturedModel*>* models, std::string filePath, std::string fileName);

//Each TexturedModel contained within 'models' must be deleted later.
//Returns 0 if successful, 1 if model is already loaded, -1 if file couldn't be loaded
int loadObjModel(std::list<TexturedModel*>* models, std::string filePath, std::string fileName);

//Each TexturedModel contained within 'models' must be deleted later.
//Returns 0 if successful, 1 if model is already loaded, -1 if file couldn't be loaded
int loadBinaryModel(std::list<TexturedModel*>* models, std::string filePath, std::string fileName);

//Each TexturedModel contained within 'models' must be deleted later.
//Returns 0 if successful, 1 if model is already loaded, -1 if file couldn't be loaded
int loadObjModelWithMTL(std::list<TexturedModel*>* models, std::string filePath, std::string fileNameOBJ, std::string fileNameMTL);

//Each TexturedModel contained within 'models' must be deleted later.
//Returns 0 if successful, 1 if model is already loaded, -1 if file couldn't be loaded
int loadBinaryModelWithMTL(std::list<TexturedModel*>* models, std::string filePath, std::string fileNameBin, std::string fileNameMTL);

//The CollisionModel returned must be deleted later.
CollisionModel* loadCollisionModel(std::string filePath, std::string fileName);

//The CollisionModel returned must be deleted later.
CollisionModel* loadBinaryCollisionModel(std::string filePath, std::string fileName);
#endif