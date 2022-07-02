#version 400 core

in vec2 position;

out vec4 clipSpace;
out vec2 textureCoords;
out vec3 toCameraVector;
out vec3 passCameraPosition;

out vec3 worldposition;

uniform mat4 projectionMatrix;
uniform mat4 viewMatrix;
uniform mat4 modelMatrix;
uniform vec3 cameraPosition;
uniform vec3 lightPosition;
uniform mat4 toShadowMapSpace;
uniform vec4 clipPlaneBehind;

// original: water tile size 1500 = 6 tiling.
// const float tiling = 6.0;

void main(void) 
{
    vec4 worldPosition = modelMatrix * vec4(position.x, 0.0f, position.y, 1.0);
    gl_ClipDistance[1] = dot(worldPosition, clipPlaneBehind);
    worldposition = worldPosition.xyz;
    clipSpace = projectionMatrix * viewMatrix * worldPosition;
    gl_Position = clipSpace;
    //textureCoords = vec2(position.x/2.0 + 0.5, position.y/2.0 + 0.5) * tiling;
    textureCoords = vec2(worldposition.x, worldposition.z)*0.0025;
    toCameraVector = cameraPosition - worldPosition.xyz;
    passCameraPosition = cameraPosition;
}
