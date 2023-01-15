# CubeMap Conversion

CubeMaps can be converted from a single texture containing all six faces. The layout of
the faces within the texture must match one of the supported layouts below. The aspect
ratio of the input texture is used to determine how faces should be read.

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
