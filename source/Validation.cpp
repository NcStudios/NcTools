#include "Validation.h"

int SanitizeFloat(float* value)
{
    if(std::isnan(*value))
    {
        *value = 0.0f;
        return 1;
    }

    return 0;
}

int SanitizeVector(aiVector3D* value)
{
    return SanitizeFloat(&value->x)
            + SanitizeFloat(&value->y)
            + SanitizeFloat(&value->z);
}

int SanitizeVectorArray(aiVector3D* arr, unsigned count)
{
    int numSanitized = 0;
    for(unsigned i = 0u; i < count; ++i)
    {
        numSanitized += SanitizeVector(&arr[i]);
    }

    return numSanitized;
}

int SanitizeMesh(aiMesh* mesh)
{
    const auto count = mesh->mNumVertices;
    return SanitizeVectorArray(mesh->mVertices, count)
         + SanitizeVectorArray(mesh->mNormals, count)
         + SanitizeVectorArray(mesh->mTextureCoords[0], count)
         + SanitizeVectorArray(mesh->mTangents, count)
         + SanitizeVectorArray(mesh->mBitangents, count);
}