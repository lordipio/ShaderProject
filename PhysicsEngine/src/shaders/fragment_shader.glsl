#version 410 core

uniform sampler2D diffuse;
uniform sampler2D overlay;
uniform vec3 tint;
uniform vec2 resolution;
uniform float currentTime;
varying vec2 vUV;
varying vec3 vVertexNormal;

float remap(float value, float inMin, float inMax, float outMin, float outMax)
{
    return (1 - ((inMax - value) / (inMax - inMin))) * (outMax - outMin) + outMin;
}

void main()
{
    // vec4 black = vec4(0, 0, 0,1);
    vec2 correctedUV = vec2(vUV.x, -vUV.y);
    // float circleRad = 3.14 * 2.f;

    // float t1 = remap(sin(vUV.y * circleRad * 100 - currentTime * 20.f), -1.f, 1.f, 0.8f, 1.f);
    // float t2 = remap(sin(vUV.y * circleRad * 250 + currentTime * 50.f), -1.f, 1.f, 0.7f, 1.f);

    // vec4 color = texture(diffuse, correctedUV);
    // color = mix(black, color, t1);
    // color = mix(black, color, t2);
    
    gl_FragColor = texture(diffuse, correctedUV);
}