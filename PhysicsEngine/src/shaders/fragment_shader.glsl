#version 410 core

uniform sampler2D diffuse;
uniform sampler2D overlay;
uniform vec3 tint;
uniform vec2 resolution;
uniform float currentTime;
uniform vec3 cameraDir;
uniform vec3 cameraPos;
uniform samplerCube skybox;

layout (location = 0) in vec2 vUV;
layout (location = 1) in vec3 vVertexNormal;
layout (location = 2) in vec3 vVertexPos;
layout (location = 3) in vec3 vDebugColor;

out vec4 fragColor;

float remap(float value, float inMin, float inMax, float outMin, float outMax)
{
    return (1 - ((inMax - value) / (inMax - inMin))) * (outMax - outMin) + outMin;
}

vec3 linearToSrgb(vec3 linear)
{

    vec3 low = 12.92 * linear;
    vec3 high = (vec3(1.055) * pow(linear, vec3(1.f/2.4))) - vec3(0.055);
    return mix(high, low, lessThanEqual(linear, vec3(0.0031308)));
}

vec3 srgbToLinear(vec3 srgb)
{
    vec3 low = srgb / 12.92;
    vec3 high = pow(((srgb+vec3(0.055))/vec3(1.055)), vec3(2.4));
    return mix(high, low, lessThanEqual(srgb, vec3(0.04045)));
}

void main()
{
    // cameraDir = normalize(-cameraPos + vVertexPos);
    vec3 cameraView = normalize(-cameraPos + vVertexPos);
    vec2 correctedUV = vec2(vUV.x, -vUV.y);
    // vec3 baseColor = vec3(0.5, 0.3, 0.3);
    // vec3 baseColor = texture2D(diffuse, correctedUV).xyz;
    vec3 baseColor = vDebugColor; 
    vec3 skyColor = vec3(0.f, 0.7f, 0.9f);
    vec3 groundColor = vec3(0.3f, 0.0f, 0.3f);
    float hemiMix = remap(vVertexNormal.y, -1, 1, 0, 1);
    vec3 hemiLight = mix(groundColor, skyColor, hemiMix);
    
    vec3 lightDir = normalize(vec3(1, 1, 0));   
    vec3 lightColor = vec3(1.f, 1.f, 0.8f);
    vec3 ambientLightColor = vec3(0.7f, 1.f, 1.f);

    float lambertian_dp = max (0, dot(lightDir, vVertexNormal));
    
    float cuttingShadow = 1.f;
    
    // if (lambertian_dp < 0.1)
    // cuttingShadow = 0.0;

    // else if (lambertian_dp < 0.2)
    // cuttingShadow = 0.2;

    // else if (lambertian_dp < 0.5)
    // cuttingShadow = 0.3;

    // if (lambertian_dp > 0.2 && lambertian_dp < 0.5)
       //  cuttingShadow = 0.3;

    
    vec3 diffuse = lambertian_dp * lightColor;

    vec3 reflection = normalize(reflect(lightDir, vVertexNormal));
    float phongSpecular = max(0.f, dot(reflection, cameraView)); 
    phongSpecular = pow(phongSpecular, 64);
    vec3 specularColor = phongSpecular * lightColor;
    specularColor = smoothstep(0.5, 0.505, specularColor);

    vec3 IBL_coord = normalize(reflect(cameraView, vVertexNormal));
    vec3 IBL_color = texture(skybox, IBL_coord).xyz;
    // specularColor += 0.2 * IBL_color;

    float fresnel = 1 - max(0, dot(-cameraView, vVertexNormal));
    fresnel = pow(fresnel, 4);
    fresnel = smoothstep(0.5, 0.505, fresnel);
    // specularColor *= fresnel;

    vec3 lighting = hemiLight * (fresnel + 0.4) + diffuse * 0.8;
    vec3 color = srgbToLinear(baseColor) *  lighting + specularColor * 1.0f;
    
    color = linearToSrgb(color);


    // color = vec3(1.f);
    // color = vec3(lambertian_dp);
    // color = vec3(vDebugColor);
    // color = vDebugColor;
    fragColor = vec4(color, 1.f);
}