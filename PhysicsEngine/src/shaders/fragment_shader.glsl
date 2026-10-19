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
const vec3 yellow = vec3(1.0, 1.0, 0.0);
const vec3 sunYellow = vec3(1.0, 1.0, 0.7);
const vec3 brown = vec3(0.65, 0.16, 0.16);
const vec3 orange = vec3(1.0, 0.5, 0.0);
const vec3 orangeRed = vec3(1.0, 0.15, 0.0);
 
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

    
vec3 ColoredBackground()
{
    vec3 topColor;
    vec3 botColor;
    topColor = mix(red, green, step(0.5, vUV.x));
    botColor = mix(yellow, blue, step(0.5, vUV.x));
    return mix(botColor, topColor, step(0.5, vUV.y));
}

float Rand(vec2 p)
{
    p = 50.0 * fract(p * 0.3183099 + vec2(0.71, 0.113));
    return -1.0 + 2.0 * fract(p.x * p.y * (p.x + p.y));
}

float noise(vec2 coord, float cellSize)
{
    vec2 pc = cellSize * coord;
    vec2 base = floor(pc);
    
    float s1 = Rand((base + vec2(0, 0)) / cellSize);
    float s2 = Rand((base + vec2(1, 0)) / cellSize);
    float s3 = Rand((base + vec2(0, 1)) / cellSize);;
    float s4 = Rand((base + vec2(1, 1)) / cellSize);;

    vec2 f = fract(pc);

    float px1 = mix(s1, s2, f.x);
    float px2 = mix(s3, s4, f.x);

    return mix(px1, px2, f.y);
}

vec3 hash3D(vec3 p) {
    p = vec3( dot(p, vec3(127.1, 311.7, 74.7)),
              dot(p, vec3(269.5, 183.3, 246.1)),
              dot(p, vec3(113.5, 271.9, 124.6)));
              
    return -1.0 + 2.0 * fract(sin(p) * 43758.5453123);
}

float noise3D(vec3 p) {
    vec3 i = floor(p);
    vec3 f = fract(p);

    vec3 u = f * f * (3.0 - 2.0 * f);

    vec3 v000 = i + vec3(0.0, 0.0, 0.0);
    vec3 v100 = i + vec3(1.0, 0.0, 0.0);
    vec3 v010 = i + vec3(0.0, 1.0, 0.0);
    vec3 v110 = i + vec3(1.0, 1.0, 0.0);
    vec3 v001 = i + vec3(0.0, 0.0, 1.0);
    vec3 v101 = i + vec3(1.0, 0.0, 1.0);
    vec3 v011 = i + vec3(0.0, 1.0, 1.0);
    vec3 v111 = i + vec3(1.0, 1.0, 1.0);

    float d000 = dot(hash3D(v000), f - vec3(0.0, 0.0, 0.0));
    float d100 = dot(hash3D(v100), f - vec3(1.0, 0.0, 0.0));
    float d010 = dot(hash3D(v010), f - vec3(0.0, 1.0, 0.0));
    float d110 = dot(hash3D(v110), f - vec3(1.0, 1.0, 0.0));
    float d001 = dot(hash3D(v001), f - vec3(0.0, 0.0, 1.0));
    float d101 = dot(hash3D(v101), f - vec3(1.0, 0.0, 1.0));
    float d011 = dot(hash3D(v011), f - vec3(0.0, 1.0, 1.0));
    float d111 = dot(hash3D(v111), f - vec3(1.0, 1.0, 1.0));

    float mixX1 = mix(d000, d100, u.x);
    float mixX2 = mix(d010, d110, u.x);
    
    float mixX3 = mix(d001, d101, u.x);
    float mixX4 = mix(d011, d111, u.x);

    float mixY1 = mix(mixX1, mixX2, u.y);
    float mixY2 = mix(mixX3, mixX4, u.y);

    return mix(mixY1, mixY2, u.z);
}

float FBM(vec3 p, int octaves, float persistence, float lacunarity)
{
    float amplitude = 0.5;
    float frequency = 1.0;
    float total = 0.0;
    float normalization = 0.0;

    for (int i = 0; i < octaves; i++)
    {
        float noiseVal = noise3D(p * frequency) * amplitude;
        total += noiseVal;
        normalization += amplitude;

        amplitude *= persistence;
        frequency *= lacunarity;
    }

    total /= normalization;
    return total;
}

float RidgedFBM(vec3 p, int octaves, float persistence, float lacunarity)
{
    float amplitude = 0.5;
    float frequency = 1.0;
    float total = 0.0;
    float normalization = 0.0;

    for (int i = 0; i < octaves; i++)
    {
        float noiseVal = noise3D(p * frequency);
        noiseVal = abs(noiseVal);
        noiseVal = 1 - noiseVal;
        total += noiseVal * amplitude;
        normalization += amplitude;

        amplitude *= persistence;
        frequency *= lacunarity;
    }

    total /= normalization;
    total *= total;
    return total;
}

float TurbulenceFBM(vec3 p, int octaves, float persistence, float lacunarity)
{
    float amplitude = 0.5;
    float frequency = 1.0;
    float total = 0.0;
    float normalization = 0.0;

    for (int i = 0; i < octaves; i++)
    {
        float noiseVal = noise3D(p * frequency);
        noiseVal = abs(noiseVal);
        total += noiseVal * amplitude;
        normalization += amplitude;

        amplitude *= persistence;
        frequency *= lacunarity;
    }

    total /= normalization;
    return total;
}

float CellularNoise(vec3 coord)
{
    vec2 i = floor(coord.xy);
    vec2 f = fract(coord.xy);

    float minDist = 10;

    for(int x = -1; x <= 1; x++)
        for(int y = -1; y <= 1; y++)
        {
            vec2 gridOffset = vec2(x, y);
            vec2 gridPos = i + gridOffset;
            vec2 randomPoint = vec2(Rand(gridPos), Rand(vec2(gridPos * 12.81302)));
            randomPoint = 0.5 + 0.5 * sin(randomPoint * 91.9123 + coord.z);
            vec2 cellPoint =  randomPoint.xy + gridPos;
            float distFromBaseCellPoint = distance(cellPoint, coord.xy);
            minDist = min(minDist, distFromBaseCellPoint);
        }

    return minDist;
}

float SteppedNoise(float noiseSample)
{
    float steppedSampled = floor(noiseSample * 10.0) / 10.0;
    float remainder = fract(noiseSample * 10.0);

    return (steppedSampled - remainder) * 0.5 + 0.5;
}

float DomainWarpingFBM(vec3 coord)
{
    vec3 offset = vec3(
        FBM(coord + vec3(0.239, 481, 29), 4, 0.5, 2),
        FBM(coord + vec3(21, 0.321, 382), 2, 0.8, 4),
        0
    );

    vec3 offset2 = vec3(
        FBM(coord + 4 * offset + vec3(9, 481.02, 183.293), 8, 0.2, 0.9),
        FBM(coord + 8 * offset + vec3(0.85, 41.02, 1.9481), 8, 0.5, 4),
        0
    );
    return FBM(coord + offset2, 8, 0.5, 2);
}

float SDFCircle(vec2 pixelCoord, float radius, vec2 pos)
{
    pixelCoord = pos - pixelCoord;
    return length(pixelCoord) - radius;
}

void main()
{
    vec2 uv = vec2(vUV.x * 2, 1 - vUV.y);
    vec3 color = texture2D(diffuse, uv).xyz;

    if (uv.x > 1.0)
    {
        // Tinting
        // color *= vec3(0.1, 0.8, 0.42);

        // Brightness
        // color += vec3(0.1);

        // Saturation
        float luminance = clamp(dot(color, vec3(0.2126, 0.7152, 0.0722)), 0, 1);
        float saturationAmount = 0.0;
        // color = mix(vec3(luminance), color, saturationAmount);

        // Contrast
        float contrastAmount = 8;
        float midPoint = 0.5;
        // color = clamp((color - midPoint) * contrastAmount + midPoint, 0, 1);

        // Color Boost
        vec3 refColor = vec3(0.1, 0.1, 1);
        // float colorWeight = 1 - distance(color, refColor);
        float colorWeight = dot(normalize(refColor), normalize(color));
        colorWeight = pow(colorWeight, 20);
        colorWeight = smoothstep(0.0, 1.0, colorWeight);
        // color = mix(vec3(luminance), color, colorWeight);

        // Vignette Effect
        vec2 vignetteUV = fract(vec2(vUV.x * 2, vUV.y));
        vignetteUV.x = 1 - abs((vignetteUV.x - 0.5) * 2);
        vignetteUV.y = 1 - abs((vignetteUV.y - 0.5) * 2);
        float vignetteAmount = vignetteUV.x + vignetteUV.y;
        vignetteAmount = remap(vignetteAmount, 0, 2, 0, 1);
        vignetteAmount = smoothstep(0.0, 1.0, vignetteAmount);
        // color *= vec3(vignetteAmount);

        // Pixelated Effect
        vec2 gridSize = vec2(80.0);
        vec2 pixelatedUV = vec2(fract(vUV.x * 2), 1 - vUV.y);
        pixelatedUV = floor(pixelatedUV * gridSize) / gridSize;
        color = texture2D(diffuse, pixelatedUV).xyz;


        // Distortion
        // vec2 pushedCoord = uv;
        // float pushedSign = sign(uv.y - 0.5);
        // color = vec3(pushedSign);
        // pushedCoord.y = pushedSign * pow(abs(pushedCoord.y - 0.5) * 2.0, 0.7) * 0.5 + 0.5;
        // color = texture2D(diffuse, pushedCoord).xyz;

        
        // Ripple
        // vec2 rippleUV = uv - vec2(1.5, 0.5);
        // float d = length(rippleUV);
        // d = sin(d * 50);
        // vec2 dir = normalize(rippleUV);
        // vec2 rippleCoord = uv + d * dir * 0.05;
        // color = texture2D(diffuse, rippleCoord).xyz;

        

    }

    gl_FragColor = vec4(color, 1); 
}