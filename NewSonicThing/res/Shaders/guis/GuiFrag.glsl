#version 140

in vec2 textureCoords;

out vec4 out_Color;

uniform sampler2D guiTexture;
uniform float alpha;

void main(void)
{
    out_Color = texture(guiTexture, textureCoords);
    out_Color.a *= alpha;
}
