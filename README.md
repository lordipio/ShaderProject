<div align="center">

# ✦ ShaderProject

### A playground for pixels, light & motion.

A collection of real-time GLSL experiments exploring procedural visuals, stylized rendering, animated geometry, and nostalgic screen effects.

<br />

[![GLSL](https://img.shields.io/badge/GLSL-Shader%20Experiments-8B5CF6?style=flat-square&logo=opengl&logoColor=white)](https://www.khronos.org/opengl/)
[![Real-time Graphics](https://img.shields.io/badge/Graphics-Real--Time-111827?style=flat-square&logo=opengl&logoColor=white)](https://www.opengl.org/)
[![Procedural Art](https://img.shields.io/badge/Visuals-Procedural-0F766E?style=flat-square)](#shader-gallery)
[![C++](https://img.shields.io/badge/Project-C%2B%2B%20%2B%20GLSL-2563EB?style=flat-square&logo=cplusplus&logoColor=white)](#getting-started)

<br />


</div>

---

## Overview

**ShaderProject** is a collection of small graphics experiments, each built around one visual idea. From strange, living surfaces to burning image transitions, CRT-inspired distortion, procedural skies, and deforming 3D forms, the project is a place to explore what can be created through shader code.

Each effect lives on its own Git branch, so you can explore the implementation of an individual experiment without mixing it with the others.

## Shader Gallery

*The previews below load directly from the repository's `master` branch, so they can also appear when this README is viewed from another branch.*

<table>
  <tr>
    <td width="50%" valign="top">
      <h3>01 / Biocreep</h3>
      <a href="https://github.com/lordipio/ShaderProject/blob/master/PhysicsEngine/assets/Biocreep.gif"><img src="https://raw.githubusercontent.com/lordipio/ShaderProject/master/PhysicsEngine/assets/Biocreep.gif" alt="Animated preview of the Biocreep shader" width="100%" /></a>
      <p><a href="https://github.com/lordipio/ShaderProject/tree/biocreep"><code>biocreep</code> ↗</a></p>
      <p>An eerie, organic surface with shifting detail and glossy highlights.</p>
      <sub><b>Focus</b> · Procedural noise · Surface normals · Lighting</sub>
    </td>
    <td width="50%" valign="top">
      <h3>02 / Burning Image</h3>
      <a href="https://github.com/lordipio/ShaderProject/blob/master/PhysicsEngine/assets/Burning%20Image.gif"><img src="https://raw.githubusercontent.com/lordipio/ShaderProject/master/PhysicsEngine/assets/Burning%20Image.gif" alt="Animated preview of the Burning Image shader" width="100%" /></a>
      <p><a href="https://github.com/lordipio/ShaderProject/tree/burning_image"><code>burning_image</code> ↗</a></p>
      <p>An image dissolves into a fiery edge of glowing embers.</p>
      <sub><b>Focus</b> · Noise masks · Dissolve thresholds · Emissive edges</sub>
    </td>
  </tr>
  <tr>
    <td width="50%" valign="top">
      <h3>03 / Cell Shading</h3>
      <a href="https://github.com/lordipio/ShaderProject/blob/master/PhysicsEngine/assets/Cellshade.gif"><img src="https://raw.githubusercontent.com/lordipio/ShaderProject/master/PhysicsEngine/assets/Cellshade.gif" alt="Animated preview of the Cell Shading shader" width="100%" /></a>
      <p><a href="https://github.com/lordipio/ShaderProject/tree/cell_shading"><code>cell_shading</code> ↗</a></p>
      <p>A graphic, illustrated look built from bold bands of light and shadow.</p>
      <sub><b>Focus</b> · Quantized lighting · Posterization · Outlines</sub>
    </td>
    <td width="50%" valign="top">
      <h3>04 / CRT Effect</h3>
      <a href="https://github.com/lordipio/ShaderProject/blob/master/PhysicsEngine/assets/CRT.gif"><img src="https://raw.githubusercontent.com/lordipio/ShaderProject/master/PhysicsEngine/assets/CRT.gif" alt="Animated preview of the CRT Effect shader" width="100%" /></a>
      <p><a href="https://github.com/lordipio/ShaderProject/tree/crt_effect"><code>crt_effect</code> ↗</a></p>
      <p>A retro display treatment inspired by the character of old television screens.</p>
      <sub><b>Focus</b> · Scanlines · Curvature · Vignette</sub>
    </td>
  </tr>
  <tr>
    <td width="50%" valign="top">
      <h3>05 / Procedural Landscape</h3>
      <a href="https://github.com/lordipio/ShaderProject/blob/master/PhysicsEngine/assets/Procedural%20Landscape.gif"><img src="https://raw.githubusercontent.com/lordipio/ShaderProject/master/PhysicsEngine/assets/Procedural%20Landscape.gif" alt="Animated preview of the Procedural Landscape shader" width="100%" /></a>
      <p><a href="https://github.com/lordipio/ShaderProject/tree/landscape"><code>landscape</code> ↗</a></p>
      <p>Layered mountain silhouettes fade into atmospheric distance.</p>
      <sub><b>Focus</b> · Fractal Brownian motion · Parallax layers · Fog</sub>
    </td>
    <td width="50%" valign="top">
      <h3>06 / Procedural Cloudy Day</h3>
      <a href="https://github.com/lordipio/ShaderProject/blob/master/PhysicsEngine/assets/Procedural%20Cloudy%20Day.gif"><img src="https://raw.githubusercontent.com/lordipio/ShaderProject/master/PhysicsEngine/assets/Procedural%20Cloudy%20Day.gif" alt="Animated preview of the Procedural Cloudy Day SDF shader" width="100%" /></a>
      <p><a href="https://github.com/lordipio/ShaderProject/tree/sdf"><code>sdf</code> ↗</a></p>
      <p>A minimal animated sky assembled from soft geometric forms.</p>
      <sub><b>Focus</b> · Signed distance fields · Smooth blending · Sky gradients</sub>
    </td>
  </tr>
  <tr>
    <td width="50%" valign="top">
      <h3>07 / Toon Shading</h3>
      <a href="https://github.com/lordipio/ShaderProject/blob/master/PhysicsEngine/assets/Toon%20Shading.gif"><img src="https://raw.githubusercontent.com/lordipio/ShaderProject/master/PhysicsEngine/assets/Toon%20Shading.gif" alt="Animated preview of the Toon Shading shader" width="100%" /></a>
      <p><a href="https://github.com/lordipio/ShaderProject/tree/toon-shading"><code>toon-shading</code> ↗</a></p>
      <p>Stylized lighting gives a 3D subject a bold, hand-illustrated feel.</p>
      <sub><b>Focus</b> · Lighting ramps · Fresnel rim light · Hemisphere color</sub>
    </td>
    <td width="50%" valign="top">
      <h3>08 / Warped Jelly</h3>
      <a href="https://github.com/lordipio/ShaderProject/blob/master/PhysicsEngine/assets/Warped%20Jelly.gif"><img src="https://raw.githubusercontent.com/lordipio/ShaderProject/master/PhysicsEngine/assets/Warped%20Jelly.gif" alt="Animated preview of the Warped Jelly shader" width="100%" /></a>
      <p><a href="https://github.com/lordipio/ShaderProject/tree/warp-sphere"><code>warp-sphere</code> ↗</a></p>
      <p>A sphere ripples and bends into a soft, jelly-like animated form.</p>
      <sub><b>Focus</b> · Vertex displacement · Wave animation · Depth-based color</sub>
    </td>
  </tr>
</table>

> **Tip:** Click any preview to open the full GIF on GitHub.


