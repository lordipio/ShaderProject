#version 410 core

uniform sampler2D diffuse;
uniform sampler2D overlay;
uniform vec3 tint;
uniform vec2 resolution;
uniform float currentTime;
varying vec2 vUV;
varying vec3 vVertexNormal;


vec3 black = vec3(0);
vec3 white = vec3(1);
vec3 grey = vec3(0.7);
vec3 red = vec3(1.0, 0.0, 0.0);
vec3 blue = vec3(0, 0.0, 1.0);
vec3 green = vec3(0, 1.0, 0.0);

float remap(float value, float inMin, float inMax, float outMin, float outMax)
{
    return (1 - ((inMax - value) / (inMax - inMin))) * (outMax - outMin) + outMin;
}

vec3 backgroundColor()
{
    float color = length(abs(vUV - vec2(0.5)));
    color = 1 - smoothstep(0.0, 0.7, color);
    color = remap(color, 0.0, 1.0, 0.4, 0.8);
    return vec3(color);
}

vec3 drawGrid()
{
    vec2 UV = vec2(vUV.x * resolution.x/resolution.y, vUV.y) - vec2(0.5 * resolution.x/resolution.y,0.5);
    vec2 color = smoothstep(vec2(0.95), vec2(1.0), fract(UV * 10.0));
    vec3 outputColor = vec3(max(color.x, color.y));
    return outputColor;    
}

float SDFCircle(vec2 pixelCoord, float radius, vec2 pos)
{
    pixelCoord = pos - pixelCoord;
    return length(pixelCoord) - radius;
}

float SDFLine(vec2 pixelCoord, vec2 a, vec2 b)
{
    vec2 ba = b-a;
    vec2 pa = pixelCoord - a;

    float t = dot(pa, ba)/dot(ba, ba);

    t = clamp(t, 0, 1);

    vec2 nearestPoint = a + t * ba;

    float distFromP = length(nearestPoint - pixelCoord);

    return distFromP;
}

float SDFBox(vec2 pixelCoord, vec2 boxSize)
{
    vec2 pos = pixelCoord ;
    vec2 d = abs(pos) - boxSize;

    float dist = length(max(d, 0.0)) + min(max(d.x, d.y), 0);
    return dist;
}

mat2 Rotate2D(float angle)
{
    float c = cos(angle);
    float s = sin(angle);

    return mat2(c, -s, s, c);
}

float SDFUnion(float d1, float d2)
{
    return min(d1, d2);
}

float SDFIntersection(float d1, float d2)
{
    return max(d1, d2);
}

float sminPoly(float a, float b, float k) 
{
    // 1. Calculate the blend percentage (h)
    float h = clamp(0.5 + 0.5 * (b - a) / k, 0.0, 1.0);
    
    // 2. Mix the two distances and subtract the mathematical curve
    return mix(b, a, h) - k * h * (1.0 - h);
}

void main()
{
    vec2 correctedUV = vec2(vUV.x, -vUV.y);
    float circleRad = 3.14 * 2.f;
    int num = 10;

    float aspectRatio = resolution.x / resolution.y; 

    vec3 color = white;

    color = backgroundColor();
    color = mix(color, vec3(0.0), drawGrid());
    // color = drawGrid();
    
    vec2 pixelCoord = vec2(vUV.x * aspectRatio, vUV.y) - vec2(0.5 * aspectRatio, 0.5);
    pixelCoord *= 10;
    
    
    

    float distToCircle1 = SDFCircle(pixelCoord, 1.2, vec2(3.0, 0));
    float distToCircle2 = SDFCircle(pixelCoord, 1.0, vec2(0, 4));
    float distToCircle3 = SDFCircle(pixelCoord, 1.5, vec2(-3.5, 0));

    vec2 boxPos = vec2(0, 0);
    boxPos = pixelCoord - boxPos;
    boxPos *= Rotate2D(currentTime);
    float distToBox = SDFBox(boxPos, vec2(3, 1));
    
    // float d = SDFIntersection(SDFIntersection(distToCircle1, distToCircle2), distToCircle3);
    // d = SDFIntersection(d, distToBox);

    float d = SDFUnion(SDFUnion(distToCircle1, distToCircle2), distToCircle3);
    d = sminPoly(d, distToBox, 0.98);

    // color = mix(red, color, step(0, circle));
    // color = mix(red, color, smoothstep(-0.01, 0.01, d));
    color = mix(red, color, smoothstep(-0.01, 0.01, d));
    color = mix(black, color, pow(smoothstep(-0.01, 0.01, abs(d)), 20));
    
    // float thickness = 0.02;
    // float line = SDFLine(pixelCoord, vec2(2, 2), vec2(6, 3));
    // color = mix(blue, color, smoothstep(thickness - 0.01, thickness + 0.01, line));

    // vec2 boxPos = vec2(2,2 ); 
    // boxPos = pixelCoord - boxPos;
    // boxPos = Rotate2D(currentTime) * boxPos;
    // float box = SDFBox(boxPos, vec2(2, 1));
    // color = mix(green, color, box);

    gl_FragColor = vec4(color, 1.0);
}