#pragma once 
#include "mt_pch.h"

std::string readShaderFile(std::string shaderFilePath);

bool loadGltfModel(tinygltf::Model &model, const char* modelDataFilePath);

void debugModel(tinygltf::Model &model);