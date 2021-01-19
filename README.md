# Polytope

This is my graphics learning project. Currently supports a mouselook based camera, colored directional lighting, and shadow mapping. It also contains a resource system, which preprocesses model files (with `assimp`) and images (with `stb_image`).

## Setup

Polytope is actually two projects, **Polytope**, under `Polytope/` and **PolytopeTools**, under `PolytopeTools/`.

### Dependencies
You'll need to gather dependencies yourself. Dependencies go in a `deps/` folder under each project root, divided into `include`, `lib/x64/` and `dll/x64/`.

For **Polytope**, you'll need:
- `cereal` (header-only)
- `glm` (header-only)
- `glad` (from [this link](https://glad.dav1d.de/#profile=compatibility&language=c&specification=gl&loader=on&api=gl%3D3.3), just add `glad.c` as a source file)
- `GLFW` (copy `glfw3.lib` into `lib/x64/`)

For **PolytopeTools**, you'll need:
- `cereal` (header-only)
- `glm` (header-only)
- `assimp` (Download the VS 2019 version, and copy `assimp-vc142-mtd.lib` into `lib/x64/` and `assimp-vc142-mtd.dll` into `dll/x64/`. 
- `stb_image` (copy the header file [here](https://github.com/nothings/stb/blob/master/stb_image.h) into `include/`)

### PolytopeTools
**PolytopeTools** preprocessses files for **Polytope**. Run it with `PolytopeTools.exe <input directory> <output directory>`. By default, the input directory is at `Polytope/resource` and **Polytope** will look for the output in `Polytope/data`. Once run, **PolytopeTools** will watch the input directory for changes.
