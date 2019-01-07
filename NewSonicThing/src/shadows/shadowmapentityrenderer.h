#ifndef SHADOWMAPENTITYRENDERER_H
#define SHADOWMAPENTITYRENDERER_H

class ShadowShader;
class Matrix4f;
class Entity;
class RawModel;
class TexturedModel;

#include <glad/glad.h>
#include <unordered_map>
#include <list>

class ShadowMapEntityRenderer
{
private:
	Matrix4f* projectionViewMatrix;
	ShadowShader* shader;

	/**
	* Binds a raw model before rendering. Only the attribute 0 is enabled here
	* because that is where the positions are stored in the VAO, and only the
	* positions are required in the vertex shader.
	*
	* @param rawModel
	*            - the model to be bound.
	*/
	void bindModel(RawModel* rawModel);

	/**
	* Prepares an entity to be rendered. The model matrix is created in the
	* usual way and then multiplied with the projection and view matrix (often
	* in the past we've done this in the vertex shader) to create the
	* mvp-matrix. This is then loaded to the vertex shader as a uniform.
	*
	* @param entity
	*            - the entity to be prepared for rendering.
	*/
	void prepareInstance(Entity* entity);

public:
	/**
	* @param shader
	*            - the simple shader program being used for the shadow render
	*            pass.
	* @param projectionViewMatrix
	*            - the orthographic projection matrix multiplied by the light's
	*            "view" matrix.
	*/
	ShadowMapEntityRenderer(ShadowShader* shader, Matrix4f* projectionViewMatrix);

	/**
	* Renders entieis to the shadow map. Each model is first bound and then all
	* of the entities using that model are rendered to the shadow map.
	*
	* @param entities
	*            - the entities to be rendered to the shadow map.
	*/
	void render(std::unordered_map<TexturedModel*, std::list<Entity*>>* entities);
};
#endif