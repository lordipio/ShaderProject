#version 410 core

uniform sampler2D diffuse;
uniform sampler2D overlay;
uniform vec3 tint;
uniform vec2 resolution;
uniform float currentTime;
varying vec2 vUV;
varying vec3 vVertexNormal;

const float PI = 3.14;

const vec3 black = vec3(0);
const vec3 white = vec3(1);
const vec3 grey = vec3(0.7);
const vec3 red = vec3(1.0, 0.0, 0.0);
const vec3 blue = vec3(0, 0.0, 1.0);
const vec3 green = vec3(0, 1.0, 0.0);
const vec3 yellow = vec3(1.0, 1.0, 0.7);
 
// --- MORNING (Soft Sunrise) ---
const vec3 morningTop = vec3(0.4, 0.6, 0.9);  // Soft sky blue
const vec3 morningBot = vec3(1.0, 0.7, 0.5);  // Peach/orange horizon
 
// --- MIDDAY (Clear Bright Sky) ---
const vec3 middayTop = vec3(0.1, 0.4, 0.8);   // Deep azure blue
const vec3 middayBot = vec3(0.5, 0.8, 1.0);   // Light airy blue

// --- EVENING (Vibrant Sunset) ---
const vec3 eveningTop = vec3(0.2, 0.2, 0.6);  // Deepening purple
const vec3 eveningBot = vec3(1.0, 0.4, 0.2);  // Fiery orange/red

// --- NIGHT (Moonlit Darkness) ---
const vec3 nightTop = vec3(0.02, 0.05, 0.15); // Deep space navy
const vec3 nightBot = vec3(0.1, 0.15, 0.3);   // Ambient dark blue

const vec3 sunNoonColor = vec3(1.0, 0.4, 0.0); // Deep, warm reddish-orange
const vec3 sunMorningColor = vec3(1.0, 0.85, 0.1); // Bright, glowing yellow-orange

const float morningTime = 5;
const float middayTime = 10;
const float eveningTime = 15;
const float nightTime = 20;

float remap(float value, float inMin, float inMax, float outMin, float outMax)
{
    return (1 - ((inMax - value) / (inMax - inMin))) * (outMax - outMin) + outMin;
}

vec3 backgroundColor(float dayTime)
{
    vec3 color = mix(vec3(0.1, 0.7, 1.0), vec3(0.4, 0.6, 0.8) * 0.8, vec3(vUV, vUV.y));
    
    
    float uvMultiplied = vUV.x * vUV.y;

    vec3 night = mix(nightBot, nightTop, uvMultiplied);
    vec3 morning = mix(morningBot, morningTop, uvMultiplied);
    vec3 midday = mix(middayBot, middayTop, uvMultiplied);
    vec3 evening = mix(eveningBot, eveningTop, uvMultiplied);

    if (dayTime < morningTime) // morning
    {
        color = mix(night, morning, smoothstep(0, 1, remap(dayTime, 0, 2, 0, 1)));
    }

    else if(dayTime < middayTime) // midday
    {
        color = mix(morning, midday, smoothstep(0, 1, remap(dayTime, morningTime, morningTime + 2, 0, 1)));
    }

    else if (dayTime < eveningTime) // evening
    {
        color = mix(midday, evening, smoothstep(0, 1, remap(dayTime, middayTime, middayTime + 2, 0, 1)));
    }

    else if (dayTime < nightTime) // night
    {
        color = mix(evening, night, smoothstep(0, 1, remap(dayTime, eveningTime, eveningTime + 2, 0, 1)));
    }
    
    vec3 outputColor = color;
    return outputColor;

    return color;
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

float SDFSubtraction(float d1, float d2)
{
    return max(d1, -d2);
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

vec2 SDFCloud(vec2 pixelCoord, float radius, vec2 pos)
{
    float c1 = SDFCircle(pixelCoord, radius * 0.9, pos + vec2(0, 0));
    float c2 = SDFCircle(pixelCoord, radius * 1.1, pos + vec2(radius * 1.2, 0));
    float c3 = SDFCircle(pixelCoord, radius * 0.9, pos + vec2( radius * 2.2, 0));

    float xOffset = 30;
    float yRadiusOffsetMultiplyer = 0.4;
    float shadow1 = SDFCircle(pixelCoord, radius * 0.9, pos - vec2(xOffset, radius * yRadiusOffsetMultiplyer));
    float shadow2 = SDFCircle(pixelCoord, radius * 1.1, pos + vec2(radius * 1.2 - xOffset, -radius * yRadiusOffsetMultiplyer));
    float shadow3 = SDFCircle(pixelCoord, radius * 0.9, pos + vec2( radius * 2.2 - xOffset, -radius * yRadiusOffsetMultiplyer));
    float c = SDFUnion(SDFUnion(c1, c2), c3);
    float shadow = SDFUnion(SDFUnion(shadow1, shadow2), shadow3);

    return vec2(c, shadow);
} 

float HashSample(vec2 seed)
{
    return sin(dot(seed, vec2(12.18402, 89.02941)));
}

float SDFSun(vec2 pixelCoord, float radius, vec2 pos)
{
    float circle = SDFCircle(pixelCoord, radius, pos);
    return circle;
}

float SDFMoon(vec2 pixelCoord, float radius, vec2 pos)
{
    float smallCircle = SDFCircle(pixelCoord, radius , pos - vec2(radius * 0.5, -radius * 0.5));
    float bigCircle = SDFCircle(pixelCoord, radius, pos);
    return SDFSubtraction(bigCircle, smallCircle);
}

float easeOutBounce(float x) {
    const float n1 = 7.5625;
    const float d1 = 2.75;

    if (x < 1.0 / d1) {
        return n1 * x * x;
    } else if (x < 2.0 / d1) {
        x -= 1.5 / d1;
        return n1 * x * x + 0.75;
    } else if (x < 2.5 / d1) {
        x -= 2.25 / d1;
        return n1 * x * x + 0.9375;
    } else {
        x -= 2.625 / d1;
        return n1 * x * x + 0.984375;
    }
}

float easeInBounce(float x) {
    return 1.0 - easeOutBounce(1.0 - x);
}

float SDFPentagram(in vec2 p, in float r, in vec2 pos)
{
    p -= pos;
    const float k1x = 0.809016994; // cos(π/ 5) = ¼(√5+1)
    const float k2x = 0.309016994; // sin(π/10) = ¼(√5-1)
    const float k1y = 0.587785252; // sin(π/ 5) = ¼√(10-2√5)
    const float k2y = 0.951056516; // cos(π/10) = ¼√(10+2√5)
    const float k1z = 0.726542528; // tan(π/ 5) = √(5-2√5)
    const vec2  v1  = vec2( k1x,-k1y);
    const vec2  v2  = vec2(-k1x,-k1y);
    const vec2  v3  = vec2( k2x,-k2y);
    
    p.x = abs(p.x);
    p -= 2.0*max(dot(v1,p),0.0)*v1;
    p -= 2.0*max(dot(v2,p),0.0)*v2;
    p.x = abs(p.x);
    p.y -= r;
    return length(p-v3*clamp(dot(p,v3),0.0,k1z*r))
           * sign(p.y*v3.x-p.x*v3.y);
}

void main()
{
    float dayTime = mod(currentTime, 20); 
    vec3 color = backgroundColor(dayTime);
    vec2 pixelCoord = (vUV - 0.5) * resolution; // -960, 960


    //////////////////////////////// SUN ////////////////////////////////
    vec2 sunStartPos = vec2(-500, 750);
    vec2 sunEndPos = vec2(-500, 270);
    vec2 sunPos = sunStartPos;
    if (dayTime < morningTime)
    {
        sunPos.y = remap(dayTime, 0, morningTime - 2, sunStartPos.y, sunEndPos.y);
        sunPos.y = clamp(sunPos.y, sunEndPos.y, sunStartPos.y);
    }

    else if (dayTime < eveningTime)
        sunPos = sunEndPos;

    else
    {
        sunPos.y = remap(dayTime, eveningTime, nightTime - 3, sunEndPos.y, sunStartPos.y);
        sunPos.y = clamp(sunPos.y, sunEndPos.y, sunStartPos.y);
    }

    float distToSun = SDFSun(pixelCoord, 120, sunPos);
    vec3 sunColor = mix(sunMorningColor, sunNoonColor, smoothstep(morningTime, middayTime - 2, dayTime));
    color = mix(sunColor, color, smoothstep(0, 50, distToSun));
    float s = max(0.001, distToSun);
    float p = exp(-0.001*s*s);
    p = clamp(p, 0, 1);
    color += 0.5 * mix(black, white, p);


    //////////////////////////////// MOON ////////////////////////////////
    vec2 moonStartPos = vec2(500, 650);
    vec2 moonEndPos = vec2(500, 370);
    vec2 moonPos = moonStartPos;
    if (dayTime > eveningTime)
    {
        float input = remap(dayTime, eveningTime, nightTime - 3, 0, 1);
        input = clamp(input, 0, 1);
        float bounceProgress = easeOutBounce(input);
        moonPos.y = mix(moonStartPos.y, moonEndPos.y, bounceProgress);
    }

    else if (dayTime < morningTime)
        moonPos.y = remap(dayTime, 0, morningTime - 3, moonEndPos.y, moonStartPos.y);

    float distToMoon = SDFMoon(pixelCoord, 80, moonPos);
    vec3 moonColor = white;
    color = mix(moonColor, color, smoothstep(0, 5, distToMoon));
    s = max(0.001, distToMoon);
    p = exp(-0.01*s*s);
    p = clamp(p, 0, 1);
    color += 0.5 * mix(black, white, p);


    //////////////////////////////// STARS ////////////////////////////////
    const int STARS_COUNT = 8;
    vec2 starEndPos;
    vec2 starStartPos;
    vec2 starPos;
    float alpha;
    float starBounceSpeed;
    for (int i = 0; i < STARS_COUNT; i++)
    {
        alpha = 1.0;
        starBounceSpeed = remap(HashSample(vec2(-1.5 * i, i * 11)), -1.0, 1.0, 0.8, 1.0);
        starEndPos.x = HashSample(vec2(i + 3, i * 2));
        starEndPos.x = remap(starEndPos.x, -1, 1, -900, 900);

        starEndPos.y = HashSample(vec2(i, 4));
        starEndPos.y = remap(starEndPos.y, -1, 1, 100, 450);

        starStartPos = vec2(starEndPos.x, 600);
        starPos = starStartPos;
        if (dayTime > eveningTime)
        {
            float input = remap(dayTime, eveningTime, nightTime * starBounceSpeed, 0, 1);
            alpha = 1.0;
            input = clamp(input, 0, 1);
            float bounceProgress = easeOutBounce(input);
            starPos.y = mix(starStartPos.y, starEndPos.y, bounceProgress);
        }

        else if (dayTime < morningTime)
        {
            alpha = 1 - clamp(remap(dayTime, 0, morningTime * starBounceSpeed * 0.5, 0, 1.0), 0, 1);
            starPos.y = starEndPos.y;
        }

        float starSize = remap(HashSample(vec2(i - 1.5, i * 20.412)), -1, 1, 5, 30);
        float distToStar = SDFPentagram(pixelCoord, starSize, starPos);
        vec3 starColor = yellow;
        float starMask = 1 - smoothstep(0, 5, distToStar);
        starMask *= alpha; 
        color = mix(color, starColor, starMask);
        s = max(0.001, distToStar);
        p = exp(-0.01*s*s);
        p = clamp(p, 0, 1);
        color += mix(black, yellow, p) * starMask * remap(sin(currentTime * 5), -1, 1, 0.4, 1.2);

    }


    //////////////////////////////// CLOUDS ////////////////////////////////

    float xBoundaryOffset = 300;
    const int cloudCount = 10;
    for (int i = 0; i < cloudCount; i++)
    {
        float randomX = HashSample(vec2(i, 12));
        float randomY = HashSample(vec2(14, i));

        // Set Y Pos boundary
        randomY = remap(randomY, -1, 1, -0.1, 0.6);

        float size = remap(randomX, -1, 1, 0.3, 0.8);
        float speed = 1 - size;
        
        vec2 cloudPos = resolution * vec2(randomX, randomY) * 0.5f;
        cloudPos += speed * vec2(currentTime * 250, 0);
        cloudPos.x = mod(cloudPos.x, resolution.x);
        cloudPos.x = remap(cloudPos.x, 0, resolution.x, -resolution.x/2 - xBoundaryOffset, resolution.x/2 + xBoundaryOffset);
        vec2 distToCloud = SDFCloud(pixelCoord, 150.0 * size, cloudPos);

        // Rendering
        color = mix(black, color, remap(smoothstep(-10.0, 10.0, distToCloud.y), 0, 1, 0.6, 1)); // shadow
        color = mix(white, color, smoothstep(-2.0, 2.0, distToCloud.x)); // cloud 
        color = mix(black, color, pow(smoothstep(-7, 7, abs(distToCloud.x)), 6)); // cloud outline
    }

    gl_FragColor = vec4(color, 1.0);
}