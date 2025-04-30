//
// Created by daisu on 30/04/2025.
//
#include "Utils.h"

#include "minilog/minilog.h"

bool endsWith(const char* s, const char* part)
{
    const size_t sLength    = strlen(s);
    const size_t partLength = strlen(part);
    return sLength < partLength ? false : strcmp(s + sLength - partLength, part) == 0;
}

std::string readShaderFile(const char* fileName)
{
    FILE* file = fopen(fileName, "r");

    if (!file) {
        LLOGW("I/O error. Cannot open shader file '%s'\n", fileName);
        return std::string();
    }

    fseek(file, 0L, SEEK_END);
    const size_t bytesinfile = ftell(file);
    fseek(file, 0L, SEEK_SET);

    char* buffer           = (char*)alloca(bytesinfile + 1);
    const size_t bytesread = fread(buffer, 1, bytesinfile, file);
    fclose(file);

    buffer[bytesread] = 0;

    static constexpr unsigned char BOM[] = { 0xEF, 0xBB, 0xBF };

    if (bytesread > 3) {
        if (!memcmp(buffer, BOM, 3))
            memset(buffer, ' ', 3);
    }

    std::string code(buffer);

    while (code.find("#include ") != code.npos) {
        const auto pos = code.find("#include ");
        const auto p1  = code.find('<', pos);
        const auto p2  = code.find('>', pos);
        if (p1 == code.npos || p2 == code.npos || p2 <= p1) {
            LLOGW("Error while loading shader program: %s\n", code.c_str());
            return std::string();
        }
        const std::string name    = code.substr(p1 + 1, p2 - p1 - 1);
        const std::string include = readShaderFile(name.c_str());
        code.replace(pos, p2 - pos + 1, include.c_str());
    }

    return code;
}

glslang_stage_t vkShaderStageFromFileName(const char* fileName)
{
    if (endsWith(fileName, ".vert"))
        return GLSLANG_STAGE_VERTEX;

    if (endsWith(fileName, ".frag"))
        return GLSLANG_STAGE_FRAGMENT;

    if (endsWith(fileName, ".geom"))
        return GLSLANG_STAGE_GEOMETRY;

    if (endsWith(fileName, ".comp"))
        return GLSLANG_STAGE_COMPUTE;

    if (endsWith(fileName, ".tesc"))
        return GLSLANG_STAGE_TESSCONTROL;

    if (endsWith(fileName, ".tese"))
        return GLSLANG_STAGE_TESSEVALUATION;

    return GLSLANG_STAGE_VERTEX;
}
