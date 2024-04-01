#version 400 core

in vec4 clipSpace;
in vec2 textureCoords;
in vec3 toCameraVector;
in vec3 passCameraPosition;
//in vec3 fromLightVector;

in vec3 worldposition;

out vec4 out_Color;
out vec4 out_BrightColor;

uniform vec3 sunDirection;
uniform sampler2D reflectionTexture;
uniform sampler2D refractionTexture;
uniform sampler2D dudvMap;
uniform sampler2D normalMap;
uniform sampler2D depthMap;
uniform vec3 sunColor;
uniform float waterHeight;
uniform float moveFactor;
uniform float murkiness;
uniform vec3 waterColor;
uniform vec3 skyColor;
uniform float fogDensity;
uniform float fogGradient;

const float NEAR = 1.5;
uniform float FAR;

const float waveStrength = 0.025;
const float shineDamper = 80.0;
const float reflectivity = 0.80;


void main(void)
{
    vec2 ndc = (clipSpace.xy/clipSpace.w)/2.0 + 0.5;
    vec2 refractTexCoords = vec2(ndc.x, ndc.y);
    vec2 reflectTexCoords = vec2(ndc.x, -ndc.y);
    
    
    float depth = texture(depthMap, refractTexCoords).r;

    //converts raw un-linear data from depthMap to the actual distance that it truly is 
    float floorDistance = (2.0*NEAR*FAR)/((FAR + NEAR) - ((2.0*depth) - 1.0)*(FAR - NEAR));
    
    depth = gl_FragCoord.z;
    float waterDistance = (2.0*NEAR*FAR)/((FAR + NEAR) - ((2.0*depth) - 1.0)*(FAR - NEAR));
    //float waterDepth = floorDistance - waterDistance;
    float waterDepthFactor = clamp((floorDistance - waterDistance)/25.0, 0.0, 1.0);
    
    //float waterDepth = floorDistance - waterDistance;
    ////float fogDepthDensity = 6;
    ////float fogDepthGradient = 0.09;
    //float fogDepthDensity = 0.0035;
    //float fogDepthGradient = 0.75;
    //float val = exp(-pow((waterDepth*fogDepthDensity), fogDepthGradient));
    //val = val*0.55 + 0.45;
    //val = clamp(val, 0, 1);
    
    //out_Color = vec4(val, val, val, 1);
    //return;
    
    //old
    //vec2 distortion1 = (texture(dudvMap, vec2(textureCoords.x + moveFactor, textureCoords.y)).rg * 2.0 -1.0) * waveStrength;
    //vec2 distortion2 = (texture(dudvMap, vec2(-textureCoords.x + moveFactor, textureCoords.y + moveFactor)).rg * 2.0 -1.0) * waveStrength;
    //vec2 totalDistortion = distortion1 + distortion2;
    
    vec2 distortedTexCoords = texture(dudvMap, vec2(textureCoords.x + moveFactor, textureCoords.y)).rg*0.1;
    distortedTexCoords = textureCoords + vec2(distortedTexCoords.x, distortedTexCoords.y+moveFactor);
    vec2 totalDistortion = (texture(dudvMap, distortedTexCoords).rg * 2.0 - 1.0) * waveStrength;//* clamp(waterDepth/20.0, 0.0, 1.0);
    
    vec3 viewVector = normalize(toCameraVector);
    float refractiveFactor = dot(viewVector, vec3(0, 1, 0));
    float steepness = abs(refractiveFactor);
    
    totalDistortion*=steepness;
    
    
    refractTexCoords += totalDistortion;
    refractTexCoords = clamp(refractTexCoords, 0.001, 0.999);
    
    reflectTexCoords += totalDistortion;
    reflectTexCoords.x = clamp(reflectTexCoords.x, 0.001, 0.999);
    reflectTexCoords.y = clamp(reflectTexCoords.y, -0.999, -0.001);
    
    vec4 reflectColor = texture(reflectionTexture, reflectTexCoords);
    vec4 refractColor = texture(refractionTexture, refractTexCoords);
    //refractColor*=0.85;
    
    vec4 normalMapColor = texture(normalMap, distortedTexCoords);
    
    vec3 normal = vec3(normalMapColor.r * 2.0 - 1.0, normalMapColor.b, normalMapColor.g * 2.0 - 1.0);
    normal = normalize(normal);
    
    //simulate a non-smooth surface
    //float offsetX = (rand(vec2(worldposition.x+random1, worldposition.z+random2))-0.5)/6.0;
    //float offsetY = (rand(vec2(worldposition.x+random2, worldposition.z+random3))-0.5)/6.0;
    //float offsetZ = (rand(vec2(worldposition.x+random3, worldposition.z+random1))-0.5)/6.0;
    //normal = vec3(random1+offsetX, random2+1+offsetY, random3+offsetZ);
    

    
    if (passCameraPosition.y <= waterHeight)
    {
        refractiveFactor = -refractiveFactor*2;
        
        reflectColor = mix(reflectColor, vec4(waterColor, 1.0), waterDepthFactor*murkiness);
    }
    else
    {
        refractColor = mix(refractColor, vec4(waterColor, 1.0), waterDepthFactor*murkiness);
    }
    //refractiveFactor = pow(refractiveFactor, 1.0);
    
    refractiveFactor*=2.0; //1.5 //todo: make this stuff load from .lvl file
    refractiveFactor = clamp(refractiveFactor, 0.4, 0.9); //0.3 0.8
    //refractiveFactor = 1.0;
    
    
    //vec3 reflectedLight = reflect(normalize(fromLightVector), normal);
    vec3 reflectedLight = reflect(sunDirection, normal);
    float specular = max(dot(reflectedLight, viewVector), 0.0);
    specular = pow(specular, shineDamper);
    
    //if(shadowCoords.z > texture(shadowMap, shadowCoords.xy).r+0.00125)
    //{
        //specular = 0;
    //}
    
    out_Color = mix(reflectColor, refractColor, refractiveFactor);
    
    //idea: only apply depth favtor to refract color
    //out_Color.r = out_Color.r * val;
    //out_Color.g = out_Color.g * val;
    //out_Color.b = out_Color.b * val;

    
    //out_Color = mix(out_Color, vec4(0.0, 0.3, 0.5, 1.0), 0.2);
    vec3 specularHighlights;
    //switch(renderMode)
    //{
        //case 0:
            specularHighlights = sunColor * specular * reflectivity; //* (waterDepthFactor;
            out_Color = out_Color + vec4(specularHighlights, 0.0);
            out_Color.a = waterDepthFactor;
        //    break;
            
        //case 1:
        //    specularHighlights = lightColor * specular * reflectivity * clamp(waterDepth/20.0, 0.0, 1.0);
        //    out_Color = mix(out_Color, vec4(81/255.0, 253/255.0, 253/255.0, 1.0), 0.4) + vec4(specularHighlights, 0.0);
        //    out_Color.a = clamp(waterDepth/5.0, 0.0, 1.0);
        //    break;
            
        //case 2:
        //    specularHighlights = lightColor * specular * reflectivity;
        //    out_Color = mix(out_Color, vec4(9/255.0, 99/255.0, 224/255.0, 1.0), 0.8) + vec4(specularHighlights, 0.0);
        //    out_Color.a = clamp(waterDepth/0.1, 0.0, 1.0);
        //    break;
            
        //case 3:
        //    out_Color = mix(reflectColor, vec4(56/255.0,170/255.0,224/255.0,1), refractiveFactor);
        //    specularHighlights = lightColor * specular * reflectivity * clamp(waterDepth/20.0, 0.0, 1.0);
        //    out_Color = out_Color + vec4(specularHighlights, 0.0);
        //    out_Color.a = 1;
        //    break;
            
        //default:
        //    break;
    //}
    
    // fog
    //out_Color = mix(vec4(skyColor, 1.0), out_Color, (1 - (1 - clamp(exp(-pow((length(toCameraVector)*fogDensity), fogGradient)), 0.0, 1.0))));
    
    float brightness = (out_Color.r * 0.2126) + (out_Color.g * 0.7152) + (out_Color.b * 0.0722);
    out_BrightColor = vec4((out_Color * brightness * brightness * brightness * brightness).rgb, 1.0);
}
