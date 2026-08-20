#version 410 core

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
attribute vec3 pos;
attribute vec2 uv;
attribute vec3 vertexNormal;
varying vec2 vUV;
varying vec3 vVertexNormal;

void main()
{
    vec4 localPos = vec4(pos, 1);
    gl_Position = projection * view * model * localPos;
    vUV = uv;
    vVertexNormal = (model * vec4(vertexNormal, 0)).xyz;
}