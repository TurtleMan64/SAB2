#ifndef SHADOWFRAMEBUFFER2_H
#define SHADOWFRAMEBUFFER2_H

#include <glad/glad.h>

/**
* The frame buffer for the shadow pass. This class sets up the depth texture
* which can be rendered to during the shadow render pass, producing a shadow
* map.
*
* @author Karl
*
*/
class ShadowFrameBuffer2
{
private:
	int WIDTH;
	int HEIGHT;
	GLuint fbo;
	GLuint shadowMap;
	
	/**
	* Creates the frame buffer and adds its depth attachment texture.
	*/
	void initialiseFrameBuffer();

	/**
	* Binds the frame buffer as the current render target.
	*
	* @param frameBuffer
	*            - the frame buffer.
	* @param width
	*            - the width of the frame buffer.
	* @param height
	*            - the height of the frame buffer.
	*/
	static void bindFrameBuffer(GLuint frameBuffer, int width, int height);

	/**
	* Creates a frame buffer and binds it so that attachments can be added to
	* it. The draw buffer is set to none, indicating that there's no colour
	* buffer to be rendered to.
	*
	* @return The newly created frame buffer's ID.
	*/
	static int createFrameBuffer();

	/**
	* Creates a depth buffer texture attachment.
	*
	* @param width
	*            - the width of the texture.
	* @param height
	*            - the height of the texture.
	* @return The ID of the depth texture.
	*/
	static int createDepthBufferAttachment(int width, int height);

public:
	/**
	* Initialises the frame buffer and shadow map of a certain size.
	*
	* @param width
	*            - the width of the shadow map in pixels.
	* @param height
	*            - the height of the shadow map in pixels.
	*/
	ShadowFrameBuffer2(int width, int height);

	/**
	* Deletes the frame buffer and shadow map texture when the game closes.
	*/
	void cleanUp();

	/**
	* Binds the frame buffer, setting it as the current render target.
	*/
	void bindFrameBuffer();

	/**
	* Unbinds the frame buffer, setting the default frame buffer as the current
	* render target.
	*/
	void unbindFrameBuffer();

	/**
	* @return The ID of the shadow map texture.
	*/
	GLuint getShadowMap();
};
#endif