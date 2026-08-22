#version 410 core

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
attribute vec3 pos;
attribute vec2 uv;
attribute vec3 vertexNormal;
layout (location = 0) out vec2 vUV;
layout (location = 1) out vec3 vVertexNormal;
layout (location = 2) out vec3 vVertexPos;

void main()
{
    vec4 localPos = vec4(pos, 1);
    vUV = uv;
    vVertexNormal = (model * vec4(vertexNormal, 0)).xyz;
    vVertexPos = (model * localPos).xyz;

    gl_Position = projection * view * model * localPos;
}