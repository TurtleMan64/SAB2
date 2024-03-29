#version 400 core

in vec3 position;
in vec2 textureCoords;
in vec3 normal;
in vec4 vertexColor;

out vec2 pass_textureCoords;
out vec4 worldPosition;
out vec3 surfaceNormal;
out vec3 toCameraVector;
//out float visibility;
out vec4 pass_vertexColor;

uniform mat4 transformationMatrix;
uniform mat4 projectionMatrix;
uniform mat4 viewMatrix;

uniform float useFakeLighting;

//for use in animation of the texture coordinates
uniform float texOffX;
uniform float texOffY;

//uniform float fogDensity;
//uniform float fogGradient;

uniform vec4 clipPlane;
uniform vec4 clipPlaneBehind;

//for playstation 1 tye effect
//vec4 snap(vec4 vertex, vec2 resolution)
//{
//    vec4 snappedPos = vertex;
//    snappedPos.xyz = vertex.xyz / vertex.w; // convert to normalised device coordinates (NDC)
//    snappedPos.xy = floor(resolution * snappedPos.xy) / resolution; // snap the vertex to the lower-resolution grid
//    snappedPos.xyz *= vertex.w; // convert back to projection-space
//    return snappedPos;
//}

void main(void)
{
    worldPosition = transformationMatrix * vec4(position, 1.0);
    vec4 positionRelativeToCam = viewMatrix * worldPosition;
    
    gl_Position = projectionMatrix * positionRelativeToCam;
    
    //gl_Position = snap(gl_Position, vec2(256/1.5, 224/1.5));
    
    gl_ClipDistance[0] = dot(worldPosition, clipPlane);
    gl_ClipDistance[1] = dot(worldPosition, clipPlaneBehind);
    
    pass_textureCoords.x = textureCoords.x+texOffX;
    pass_textureCoords.y = textureCoords.y+texOffY;

    surfaceNormal = (transformationMatrix * vec4(normal, 0.0)).xyz;
    
    if (useFakeLighting > 0.5)
    {
        surfaceNormal = vec3(0.0, 1.0, 0.0);
    }
    
    //toLightVector = lightPosition - worldPosition.xyz;
    toCameraVector = (inverse(viewMatrix) * vec4(0.0, 0.0, 0.0, 1.0)).xyz - worldPosition.xyz;
    //toCameraVector = positionRelativeToCam.xyz - worldPosition.xyz;
    //toCameraVector = positionRelativeToCam.xyz;
    
    //float distance = length(positionRelativeToCam.xyz);
    //float distance = length(toCameraVector);
    //visibility = exp(-pow((distance*fogDensity), fogGradient));
    //visibility = clamp(visibility, 0.0, 1.0);
    
    pass_vertexColor = vertexColor;
}
