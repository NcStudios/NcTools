# NcTools
[![Tests](https://github.com/NcStudios/NcTools/actions/workflows/build.yml/badge.svg)](https://github.com/NcStudios/NcTools/actions?query=workflow%3ABuild)
[![License](https://img.shields.io/github/license/NcStudios/NcTools.svg)](https://github.com/NcStudios/NcTools/blob/main/LICENSE)

A collection of tools for managing and converting NcEngine assets, or .nca files.

NcTools consists of two projects: the `NcAsset` library for importing .nca
asset files, and the `nc-convert` command line utility for converting asset
files to the .nca format.

## NcAsset Overview
To use `NcAsset`, simply call the appropriate import function, and provide the
path to the .nca file you want to import:
```cpp
#include "ncasset/Import.h"

auto myTexture = nc::asset::ImportTexture("path/to/texture.nca");
```

## nc-convert Overview
`nc-convert` is used to convert various file types to the .nca format, which is
required by `NcAsset`. Here's an example of converting a single file to an .nca:
```
> nc-convert -t mesh -s path/to/input.fbx -n myMesh -o outDir
```

The asset type, source file, and output asset name must be supplied with the -t,
-s, and -n flags, respectively. Optionally, and output directory may be given
with -o.

You can also use a JSON manifest file to convert many files at once:

```json
// manifest.json
{
    "globalOptions": {
        "outputDirectory": "./",
        "workingDirectory": "./"
    },
    "mesh": [
        {
            "sourcePath": "path/to/mesh1.fbx",
            "assetName": "myMesh1"
        },
        {
            "sourcePath": "path/to/mesh2.fbx",
            "assetName": "myMesh2"
        }
    ],
    "texture": [
        {
            "sourcePath": "path/to/texture.png",
            "assetName": "myTexture"
        }
    ]
}
```

```
> nc-convert -m manifest.json
```

`nc-convert` will skip files that are already up-to-date when using a manifest.

For more information, see the help text for `nc-convert` and the docs on [input file
requirements](docs/SourceFileRequirements.md) and [.nca formats](docs/AssetFormats.md)

[NcCommon]() is a public dependency of NcAsset. The following third-party libraries are used internally by nc-convert: [Assimp](https://github.com/assimp/assimp), [AudioFile](https://github.com/adamstark/AudioFile), and [stb](https://github.com/nothings/stb).

## Build Options
-----------------
#### NC_TOOLS_BUILD_CONVERTER
    Default: OFF
    Build the nc-convert executable.

#### NC_TOOLS_BUILD_TESTS
    Default: OFF
    Build tests

#### NC_TOOLS_STATIC_ANALYSIS
    Default: OFF
    Enable static analysis (MSVC Only)
