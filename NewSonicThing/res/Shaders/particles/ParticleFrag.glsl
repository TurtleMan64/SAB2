#version 140

out vec4 out_color;

in vec2 textureCoords1;
in vec2 textureCoords2;
in float blend;

uniform sampler2D particleTexture;
uniform float brightness;
uniform float opacity;
uniform float glow;

void main(void)
{
    vec4 color1 = texture(particleTexture, textureCoords1);
    vec4 color2 = texture(particleTexture, textureCoords2);
    
    out_color = mix(color1, color2, blend);
    
    if (glow > 0.5)
    {
        out_color = vec4(out_color.rgb, out_color.a*opacity);
    }
    else
    {
        out_color = vec4(out_color.rgb*brightness, out_color.a*opacity);
    }
    
    //if(glow > 0.5)
    //{
        //out_color = out_color*((1.0)/(max(out_color.b, max(out_color.r, out_color.g))));
    //}
}
