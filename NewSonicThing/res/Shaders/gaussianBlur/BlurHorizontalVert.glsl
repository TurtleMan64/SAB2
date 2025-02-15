#version 150

in vec2 position;

out vec2 blurTextureCoords[11];

uniform float targetWidth;

void main(void)
{
    gl_Position = vec4(position, 0.0, 1.0);
    vec2 centerTexCoords = position * 0.5 + 0.5;
    float pixelSize = 1.0 / targetWidth;
    
    //for (int i=-5; i<5; i++)
    //{
    //    blurTextureCoords[i+5] = centerTexCoords + vec2(pixelSize * i, 0.0);
    //}
    
    blurTextureCoords[ 0] = centerTexCoords + vec2(pixelSize * (-5), 0.0);
    blurTextureCoords[ 1] = centerTexCoords + vec2(pixelSize * (-4), 0.0);
    blurTextureCoords[ 2] = centerTexCoords + vec2(pixelSize * (-3), 0.0);
    blurTextureCoords[ 3] = centerTexCoords + vec2(pixelSize * (-2), 0.0);
    blurTextureCoords[ 4] = centerTexCoords + vec2(pixelSize * (-1), 0.0);
    blurTextureCoords[ 5] = centerTexCoords + vec2(pixelSize * ( 0), 0.0);
    blurTextureCoords[ 6] = centerTexCoords + vec2(pixelSize * ( 1), 0.0);
    blurTextureCoords[ 7] = centerTexCoords + vec2(pixelSize * ( 2), 0.0);
    blurTextureCoords[ 8] = centerTexCoords + vec2(pixelSize * ( 3), 0.0);
    blurTextureCoords[ 9] = centerTexCoords + vec2(pixelSize * ( 4), 0.0);
    blurTextureCoords[10] = centerTexCoords + vec2(pixelSize * ( 5), 0.0);
}
