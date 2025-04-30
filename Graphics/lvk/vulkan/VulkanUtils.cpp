//
// Created by daisu on 29/04/2025.
//
#include "VulkanUtils.h"

#include <cassert>

#include <ldrutils/lutils/ScopeExit.h>

lvk::Result lvk::compileShader(glslang_stage_t stage,
                               const char* code,
                               std::vector<uint8_t>* outSPIRV,
                               const glslang_resource_t* glslLangResource)
{
  if (!outSPIRV) {
    return Result(Result::Code::ArgumentOutOfRange, "outSPIRV is NULL");
  }

  const glslang_input_t input = {
      .language = GLSLANG_SOURCE_GLSL,
      .stage = stage,
      .client = GLSLANG_CLIENT_VULKAN,
      .client_version = GLSLANG_TARGET_VULKAN_1_3,
      .target_language = GLSLANG_TARGET_SPV,
      .target_language_version = GLSLANG_TARGET_SPV_1_6,
      .code = code,
      .default_version = 100,
      .default_profile = GLSLANG_NO_PROFILE,
      .force_default_version_and_profile = false,
      .forward_compatible = false,
      .messages = GLSLANG_MSG_DEFAULT_BIT,
      .resource = glslLangResource,
  };

  glslang_shader_t* shader = glslang_shader_create(&input);
  SCOPE_EXIT {
    glslang_shader_delete(shader);
  };

  if (!glslang_shader_preprocess(shader, &input)) {
    LLOGW("Shader preprocessing failed:\n");
    LLOGW("  %s\n", glslang_shader_get_info_log(shader));
    LLOGW("  %s\n", glslang_shader_get_info_debug_log(shader));
    lvk::logShaderSource(code);
    assert(false);
    return Result(Result::Code::RuntimeError, "glslang_shader_preprocess() failed");
  }

  if (!glslang_shader_parse(shader, &input)) {
    LLOGW("Shader parsing failed:\n");
    LLOGW("  %s\n", glslang_shader_get_info_log(shader));
    LLOGW("  %s\n", glslang_shader_get_info_debug_log(shader));
    lvk::logShaderSource(glslang_shader_get_preprocessed_code(shader));
    assert(false);
    return Result(Result::Code::RuntimeError, "glslang_shader_parse() failed");
  }

  glslang_program_t* program = glslang_program_create();
  glslang_program_add_shader(program, shader);

  SCOPE_EXIT {
    glslang_program_delete(program);
  };

  if (!glslang_program_link(program, GLSLANG_MSG_SPV_RULES_BIT | GLSLANG_MSG_VULKAN_RULES_BIT)) {
    LLOGW("Shader linking failed:\n");
    LLOGW("  %s\n", glslang_program_get_info_log(program));
    LLOGW("  %s\n", glslang_program_get_info_debug_log(program));
    assert(false);
    return Result(Result::Code::RuntimeError, "glslang_program_link() failed");
  }

  glslang_spv_options_t options = {
      .generate_debug_info = true,
      .strip_debug_info = false,
      .disable_optimizer = false,
      .optimize_size = true,
      .disassemble = false,
      .validate = true,
      .emit_nonsemantic_shader_debug_info = false,
      .emit_nonsemantic_shader_debug_source = false,
  };

  glslang_program_SPIRV_generate_with_options(program, input.stage, &options);

  if (glslang_program_SPIRV_get_messages(program)) {
    LLOGW("%s\n", glslang_program_SPIRV_get_messages(program));
  }

  auto spirv = reinterpret_cast<const uint8_t*>(glslang_program_SPIRV_get_ptr(program));
  const size_t numBytes = glslang_program_SPIRV_get_size(program) * sizeof(uint32_t);

  *outSPIRV = std::vector(spirv, spirv + numBytes);

  return Result();
}