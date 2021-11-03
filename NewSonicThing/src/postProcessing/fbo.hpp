#ifndef FBO_H
#define FBO_H

#include <glad/glad.h>
#include <vector>

class Fbo
{
private:
    int width;
    int height;

    int depthBufferType;

    GLuint frameBuffer;

    bool multisampleAndMultiTarget;
    
    GLuint colorTexture;
    GLuint depthTexture;

    GLuint depthBuffer;
    GLuint colorBuffer;
    GLuint colorBuffer2;
    
public:
    static int NONE;
    static int DEPTH_TEXTURE;
    static int DEPTH_RENDER_BUFFER;

    Fbo(int width, int height, int depthBufferType);
    
    Fbo(int width, int height);
    
    void cleanUp();

    void resize(int newWidth, int newHeight);
    
    void bindFrameBuffer();
    
    void unbindFrameBuffer();
    
    //void bindToRead();
    
    GLuint getColorTexture();
    
    //int getDepthTexture();
    
    void resolveToFbo(int readBuffer, Fbo* outputFbo);
    
    //void resolveToScreen();
    
private:    
    void initialiseFrameBuffer(int type);
    
    void createFrameBuffer();
    
    void determineDrawBuffers();
    
    void createTextureAttachment();
    
    void createDepthTextureAttachment();
    
    GLuint createMultisampleColorAttatchment(int attachment);
    
    void createDepthBufferAttachment();
};
#endif
