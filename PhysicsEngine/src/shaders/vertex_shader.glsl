#version 410 core

uniform float currentTime;
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
attribute vec3 pos;
attribute vec2 uv;
attribute vec3 vertexNormal;
layout (location = 0) out vec2 vUV;
layout (location = 1) out vec3 vVertexNormal;
layout (location = 2) out vec3 vVertexPos;
layout (location = 3) out vec3 vDebugColor;

float remap(float value, float inMin, float inMax, float outMin, float outMax)
{
    return (1 - ((inMax - value) / (inMax - inMin))) * (outMax - outMin) + outMin;
}

void main()
{
    vec3 localPos = pos;
    float t = sin(localPos.y * 10.f + currentTime * 10.f);
    t = remap(t, -1, 1, 0.0, 0.3);
    
    localPos += t * vertexNormal;

    vec3 blubColor = vec3(0.3, 0.3, 1.f);
    vec3 blubColorHeight = vec3(0.6, 0.6, 1.f);
    vDebugColor = mix(blubColor, blubColorHeight, smoothstep(0.0, 0.3, t));

    vec4 finalPos = vec4(localPos, 1);
    vUV = uv;
    vVertexNormal = (model * vec4(vertexNormal, 0)).xyz;
    vVertexPos = (model * finalPos).xyz;


    gl_Position = projection * view * model * finalPos;
}