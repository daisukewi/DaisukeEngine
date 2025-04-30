//
// Created by daisu on 30/04/2025.
//
#pragma once

#include <string.h>
#include <string>

#include <glslang/Include/glslang_c_interface.h>

bool endsWith(const char* s, const char* part);

std::string readShaderFile(const char* fileName);

glslang_stage_t vkShaderStageFromFileName(const char* fileName);