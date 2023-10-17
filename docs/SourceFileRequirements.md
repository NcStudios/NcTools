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
file is ignored. For `mesh` asset types, if nc-convert is called from the command line, only the first mesh in an input
file will be processed. If the manifest is used, either the first mesh in an input file is processed, or every mesh in an 
input file is processed if the internal mesh names are specified.

It is usually recommended for the input geometry to be centered around the origin.
This makes `Transform` operations within NcEngine less surprising. Additionally,
NcEngine will treat the origin as the object's center of mass for physics
calculations.

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
