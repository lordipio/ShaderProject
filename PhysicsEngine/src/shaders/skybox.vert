#version 410 core

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
attribute vec3 pos;
attribute vec2 uv;
attribute vec3 vertexNormal;
varying vec2 vUV;
varying vec3 vVertexNormal;

layout(location = 1) out vec3 textureDir;

void main()
{
    
    vec4 localPos = vec4(pos, 1);
    vec4 finalPos = projection * view * model * localPos;
    gl_Position = finalPos;
    vUV = uv;
    vVertexNormal = vertexNormal;
    textureDir = vec3(pos);
}