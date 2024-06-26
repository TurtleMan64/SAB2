#version 400 core

in vec2 pass_textureCoords;
in vec4 worldPosition;
in vec3 surfaceNormal;
//in vec3 toLightVector;
in vec3 toCameraVector;
//in float visibility;
in vec4 pass_vertexColor;

out vec4 out_Color;


//const vec2 WINDOW_SIZE; //gets replaced in code before compiling

uniform sampler2D textureSampler;
uniform sampler2D textureSampler2;
uniform sampler2D depthBufferTransparent;
uniform int   isRenderingTransparent;
uniform int   isRenderingDepth;
uniform vec3  sunDirInv;
uniform vec3  sunColor;
uniform float shineDamper;
uniform float reflectivity;
uniform float useFakeLighting;
uniform vec3  skyColor;
uniform int   hasTransparency;
uniform float glowAmount;
uniform vec3  baseColor;
uniform float baseAlpha;
uniform float mixFactor;
uniform vec3  waterColor;
uniform float waterBlendAmount;

uniform float fogDensity;
uniform float fogGradient;
uniform float fogScale; //per material
uniform float fogBottomPosition;
uniform float fogBottomThickness;

float CalcMipLevel(sampler2D tex, vec2 uv)
{
    return textureQueryLod(tex, uv).x;
}

void main(void)
{
    if (isRenderingDepth != 0)
    {
        //uint depth = floatBitsToInt(gl_FragCoord.z/gl_FragCoord.w);
        
        //uint intR = (((depth    )    )    ) % 256;
        //uint intG = (((depth/256)    )    ) % 256;
        //uint intB = (((depth/256)/256)    ) % 256;
        //uint intA = (((depth/256)/256)/256) % 256;
        
        //out_Color = vec4(intR/255.0f, intG/255.0f, intB/255.0f, intA/255.0f);
        return;
    }
    else if (isRenderingTransparent != 0)
    {
        //float closestDepth = texture(depthBufferTransparent, vec2(gl_FragCoord.x/WINDOW_SIZE.x, gl_FragCoord.y/WINDOW_SIZE.y)).r;
        float closestDepth = texelFetch(depthBufferTransparent, ivec2(gl_FragCoord.xy), 0).r;
        
        //float offset = 0.00005;
        //float linear = (gl_FragCoord.z/gl_FragCoord.w);
        //float offset = 0.00005 * (gl_FragCoord.z+10);
        //float val = mod(closestDepth*0.5, 1.0);
        //float val = (gl_FragCoord.z/gl_FragCoord.w);
        //val = mod(val*0.1, 1.0);
        //out_Color.rbga = vec4(val, val, val, 1.0);
        //return;
        
        //some ambient occlusion testing
        //float mydepth = texelFetch(depthBufferTransparent, ivec2(gl_FragCoord.x         , gl_FragCoord.y),          0).r;
        //float factor = 1.0;
        //const int num = 10;
        //for (int x = -num; x < num; x++)
        //{
        //    for (int y = 1; y < num; y++)
        //    {
        //        float depth1 = texelFetch(depthBufferTransparent, ivec2(gl_FragCoord.x-x, gl_FragCoord.y-y), 0).r;
        //        float depth2 = texelFetch(depthBufferTransparent, ivec2(gl_FragCoord.x+x, gl_FragCoord.y+y), 0).r;
        //        if (depth1 < mydepth && depth2 < mydepth)
        //        {
        //            factor -= 1 / (num*num*0.5);
        //        }
        //    }
        //}
        
        //out_Color.rbga = vec4(factor, factor, factor, 1.0);
        //return;
        //rawTextureColor = vec4(rawTextureColor.r*factor, rawTextureColor.g*factor, rawTextureColor.b*factor, rawTextureColor.a);
        
        if (!(closestDepth > gl_FragCoord.z - 0.00005))
        {
            discard;
        }
    }
    
    vec4 rawTextureColor = texture(textureSampler, pass_textureCoords);
    if (mixFactor != 0.0)
    {
        rawTextureColor = mix(rawTextureColor, texture(textureSampler2, pass_textureCoords), mixFactor);
    }
    rawTextureColor.rgb *= baseColor*pass_vertexColor.rgb;
    
    const float ALPHA_CUTOFF = 0.2;
    //const float MipScale = 0.25;
    //
    //ivec2 tsize = textureSize(textureSampler, 0);
    //
    //vec2 TexelSize = vec2(float(tsize.x), float(tsize.y));
    //
    //rawTextureColor.a = (rawTextureColor.a - ALPHA_CUTOFF) / max(fwidth(rawTextureColor.a), 0.0001) + 0.5;
    //rawTextureColor.a *= 1 + CalcMipLevel(textureSampler, pass_textureCoords*TexelSize)*MipScale;
    
    rawTextureColor.a   *= baseAlpha*pass_vertexColor.a*0.5; //for some bizarre reason, 0.5 alpha is seen as full opaque...

    
    if (hasTransparency == 0)
    {
        if (rawTextureColor.a < ALPHA_CUTOFF) //0.45
        {
            discard;
        }
        rawTextureColor.a = 1;
    }
    else if (rawTextureColor.a == 0)
    {
        discard;
    }
    
    vec3 unitNormal = normalize(surfaceNormal);
    //vec3 unitLightVector = normalize(toLightVector);
    
    float nDotl = dot(unitNormal, sunDirInv);
    float brightness = nDotl*0.5+0.5;  //two different types of lighting options
    //float brightness = max(nDotl, 0.5);  //two different types of lighting options
    
    //make full brightness if glow
    if (useFakeLighting > 0.5)
    {
        brightness = 1.0;
    }
    
    //make more ambient light happen during the daytime, less at night
    float ambientLight = 0.5+0.5*dot(vec3(0, 1, 0), sunDirInv);
    ambientLight = (0.02+ambientLight*0.3);
    
    
    vec3 diffuse = max(brightness * sunColor, ambientLight * sunColor);
    
    vec3 unitVectorToCamera = normalize(toCameraVector);
    vec3 lightDirection = -sunDirInv;
    vec3 reflectedLightDirection = reflect(lightDirection, unitNormal);
    
    float specularFactor = dot(reflectedLightDirection, unitVectorToCamera);
    specularFactor = max(specularFactor, 0.0);
    float dampedFactor = pow(specularFactor, shineDamper);
    vec3 finalSpecular = dampedFactor * reflectivity * sunColor;
    
    //if (glowAmount > 0.0)
    //{
        //diffuse = vec3(glowAmount);
    //}
    //same as above, but no branching
    diffuse = diffuse*((floatBitsToInt(glowAmount-0.001) >> 31) & 1) + vec3(glowAmount)*((floatBitsToInt(0.001-glowAmount) >> 31) & 1);
    
    out_Color = vec4(diffuse, 1.0)*rawTextureColor + vec4(finalSpecular, rawTextureColor.a);
    
    
    //float distance = length(toCameraVector);
    //float visibility = exp(-pow((distance*fogDensity), fogGradient));
    //visibility = clamp(visibility, 0.0, 1.0);
    //visibility = (1 - fogScale*(1 - visibility));
    //out_Color = mix(vec4(skyColor, 1.0), out_Color, visibility);
    
    //condensed version of above
    out_Color = mix(vec4(skyColor, 1.0), out_Color, (1 - fogScale*(1 - clamp(exp(-pow((length(toCameraVector)*fogDensity), fogGradient)), 0.0, 1.0))));
    
    //bottom fog
    out_Color.rgb = mix(out_Color.rgb, skyColor, fogScale*(0.5 - 0.5*sin(3.141592653589*((clamp((fogBottomPosition-worldPosition.y)*fogBottomThickness, 0.0, 1.0)) + 0.5))));
    
    //water
    out_Color.rgb = mix(out_Color.rgb, waterColor, waterBlendAmount);
    
    //float z = (gl_FragCoord.z/gl_FragCoord.w)*0.001;
    //float z = (gl_FragCoord.z);
    //out_Color = vec4(z, z, z, rawTextureColor.a);
}
