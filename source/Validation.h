#pragma once

#include "Common.h"

int SanitizeFloat(float* value);
int SanitizeVector(aiVector3D* value);
int SanitizeVectorArray(aiVector3D* arr, unsigned count);
int SanitizeMesh(aiMesh* mesh);