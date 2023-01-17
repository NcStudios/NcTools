# NcTools
[![Tests](https://github.com/NcStudios/NcTools/actions/workflows/build.yml/badge.svg)](https://github.com/NcStudios/NcTools/actions?query=workflow%3ABuild)
[![License](https://img.shields.io/github/license/NcStudios/NcTools.svg)](https://github.com/NcStudios/NcTools/blob/main/LICENSE)

Tools for working with NcEngine asset and package files.

NcTools consists of two projects: the `NcAsset` library for importing .nca
asset files, and the `nc-convert` command line utility for converting asset
files to the .nca format.

## NcAsset Overview
Using `NcAsset` is straight forward - just call the appropriate import
function with the path to an .nca file:
```cpp
#include "ncasset/Import.h"

auto myTexture = nc::asset::ImportTexture("path/to/texture.nca");
```

## nc-convert Overview
Importing with `NcAsset` requires files in .nca format. `nc-convert` can be used
to generate files in this format from input files of various types, such as .wav,
.png, .fbx, and so on. An example of building a single .nca:
```
> nc-convert -t mesh -s path/to/input.fbx -n myMesh -o outDir
```

The asset type, source file, and output asset name must be supplied with the -t,
-s, and -n flags, respectively. Optionally, and output directory may be given
with -o.

Usually, many assets will need to be built at the same time. To accomplish this,
a json manifest can be supplied with -m flag:

```json
// manifest.json
{
    "globalOptions": {
        "outputDirectory": "./",
        "workingDirectory": "./"
    },
    "assets": [
        {
            "type": "mesh",
            "sourcePath": "path/to/mesh.fbx",
            "assetName": "myMesh"
        }
    ]
}
```

```
> nc-convert -m manifest.json
```

In manifest mode, `nc-convert` has the additional benefit of skipping conversions
when the .nca already exists and is up-to-date relative to its associated input file.

For more information, see the help text for `nc-convert` and the docs on [input file
requirements](docs/SourceFileRequirements.md) and [.nca formats](docs/AssetFormats.md)

[NcCommon]() is a public dependency of NcAsset. The following third-party libraries are used internally by nc-convert: [Assimp](), [AudioFile](), and [stb_image]().

## Build Options
-----------------
#### NC_TOOLS_BUILD_CONVERTER
    Default: OFF
    Build nc-convert executable.

#### NC_TOOLS_BUILD_TESTS
    Default: OFF
    Build tests

#### NC_TOOLS_STATIC_ANALYSIS
    Default: OFF
    Enable static analysis (MSVC Only)
