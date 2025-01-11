# Procedural Terrain Engine
This is an OpenGL terrain engine that aims specialized implementation of runtime virtual texturing for terrain. It is inspired by the Frostbite Engine's terrain and also John Carmack's Id Tech Engine for virtual textures. For the geometry part, I made a little change on Hugues Hoppe’s clipmap rendering technique. Streaming virtual textures are also used for heightmap. Those techniques allows to render large kilometer squares of areas with high visual quality.

### ScreenShots
![scene_0](screenshot/scene_0.png)
![scene_1](screenshot/scene_1.png)
![scene_2](screenshot/scene_2.png)

### Clipmap Rendering
![wireframe_0](screenshot/wireframe_0.png)
![wireframe_1](screenshot/wireframe_1.png)

## Runtime Virtual Textures
![physical_pages](screenshot/physical_pages.png)
![physical_pages_1](screenshot/physical_pages_1.png)

## Slope Based Dampening
![slope_based_dampening](screenshot/slope_based_dampening.png)

## How To Build
It is a CMake project. There is no external dependencies. It is tested on Windows machine.

## How To Use
Right mouse click allows to fly on the terrain. W: Go forward. S: Go backward. A: Go left. D: Go Right. E: Elevate. Q: Descend.

## Future Plans
* Terrain blended objects like rocks.
* Vegetation (Tree, bush and grass).
* Lakes and sea.
* Volumetric Clouds.
* Decals.

## References
* Clipmap rendering using nested grids. Reference : https://developer.nvidia.com/gpugems/gpugems2/part-i-geometric-complexity/chapter-2-terrain-rendering-using-gpu-based-geometry
* Virtual texturing for heightmaps. Reference: https://notkyon.moe/vt/Clipmap.pdf
* Terrain in Battlefield 3: A modern, complete and scalable system. Reference: https://media.contentapi.ea.com/content/dam/eacom/frostbite/files/gdc12-terrain-in-battlefield3.pdf
* Terrain Procedural Tools in Frostbite. Reference: https://www.youtube.com/watch?v=tBXzyoK4GvE&ab_channel=GDC2025
* Procedural shader splatting. Reference: https://media.contentapi.ea.com/content/dam/eacom/frostbite/files/chapter5-andersson-terrain-rendering-in-frostbite.pdf
* Nice terrain material tutorial series: https://www.youtube.com/watch?v=yCRzOdo4b68&t=8s&ab_channel=UnrealSensei
* FXAA implementation: https://github.com/mitsuhiko/webgl-meincraft
* OpenGL tutorial: https://learnopengl.com