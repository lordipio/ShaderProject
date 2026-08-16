#version 410 core

uniform sampler2D diffuse;
uniform samplerCube cubemap;
varying vec2 vUV;

layout (location = 1) in vec3 textureDir;

float remap(float value, float inMin, float inMax, float outMin, float outMax)
{
    return (1 - ((inMax - value) / (inMax - inMin))) * (outMax - outMin) + outMin;
}

void main()
{
    // vec2 correctedUV = vec2(vUV.x, -vUV.y);
    gl_FragColor = texture(cubemap, textureDir);
}