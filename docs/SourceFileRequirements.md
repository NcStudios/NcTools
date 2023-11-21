Source File Requirements
========================

## Audio Conversion
> Supported file types: .wav

Audio clips can be converted from any stereo or mono WAV file.

## Geometry Conversion
> Supported file types: .fbx, .obj

`concave-collider`, `hull-collider`, and `mesh` asset types are all generated from
geometry data, and therefore have similar requirements. For `concave-collider` and `hull-collider`, only the first
mesh in an input file will be processed, and non-geometry data in the input
file is ignored. For `mesh` asset types, if nc-convert is called in single-target mode, only the first mesh in an input
file will be processed. If the manifest is provided, either the first mesh in an input file is processed, or every mesh in an 
input file is processed if the sub-resource names are specified.

It is usually recommended for the input geometry to be centered around the origin.
This makes `Transform` operations within NcEngine less surprising. Additionally,
NcEngine will treat the origin as the object's center of mass for physics
calculations.

If the mesh is intended for animation, it needs to have the following properties:

- An armature with bones
- All bone weights for each vertex must be normalized to sum 1.0
- No more than four bone influences per vertices

These properties can be set in the modeling software. To set them in Blender, for example:

Normalizing bone weights:
1. Select the mesh
2. Change mode to Weight Paint
3. Select all vertices
4. Choose Weights -> Normalize All

Limiting bone influences to four:
1. Select the mesh
2. Change mode to Weight Paint
3. Select all vertices
4. Choose Weights -> Limit Total
5. Set the limit to 4 in the popup menu

Geometry used for `hull-collider` generation should be convex.

## Image Conversion
> Supported file types: .png, .jpg, .bmp

`cube-map` and `texture` asset types can both be converted from images containing
any number of RGBA channels. Any components not present will be filled with
'empty' values (e.g. opaque values for alpha channel).

Converting a `cube-map` requires a single input image containing all six faces.
The layout of the faces within the image must match one of the supported layouts
below. The aspect ratio of the input image is used to determine how faces should
be read.

Horizontal array layout (6:1):
```
[+X][-X][+Y][-Y][+Z][-Z]
```

Vertical array layout (1:6):
```
[+X]
[-X]
[+Y]
[-Y]
[+Z]
[-Z]
```

Horizontal cross layout (4:3):
```
    [+Y]
[-X][+Z][+X][-Z]
    [-Y]
```

Vertical cross layout (3:4):
```
    [+Y]
[-X][+Z][+X]
    [-Y]
    [-Z]
```

## Skeletal Animation Conversion
> Supported file types: .fbx

`skeletal-animation` assets can be converted from .FBX files with animation data.
When exporting the .fbx from the modeling software, export with scaling set to "FBX Units Scale" (in Blender) or the equivalent in other modeling software.
There is also support for external animations such as Mixamo.