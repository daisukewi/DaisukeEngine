//
// Created by daisu on 29/04/2025.
//

#pragma once

#include <cstdint>
#include <vector>

#include "../LVK.h"
#include <glslang/Include/glslang_c_interface.h>
#include <glslang/Include/glslang_c_shader_types.h>

namespace lvk {

    Result compileShader(glslang_stage_t stage,
                     const char* code,
                     std::vector<uint8_t>* outSPIRV,
                     const glslang_resource_t* glslLangResource = nullptr);

}
