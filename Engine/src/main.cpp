#include <iostream>
#include <cstdio>
#include <fstream>
#include <vector>

#include <Graphics/Utils.h>

#include <Graphics/HelpersGLFW.h>
#include <taskflow/taskflow.hpp>
#include <taskflow/algorithm/for_each.hpp>

#include <glslang/glslang/Public/resource_limits_c.h>

#include <Graphics/lvk/LVK.h>
#include <Graphics/lvk/vulkan/VulkanUtils.h>

void saveSPIRVBinaryFile(const char* filename, const uint8_t* code, size_t size)
{
    FILE* f = fopen(filename, "wb");

    if (!f) {
        LLOGW("Failed to open file. Error: %d (%s)", errno, strerror(errno));
        return;
    }

    fwrite(code, sizeof(uint8_t), size, f);
    fclose(f);
}

void testShaderCompilation(const char* sourceFilename, const char* destFilename)
{
    std::string shaderSource = readShaderFile(sourceFilename);

    assert(!shaderSource.empty());

    std::vector<uint8_t> spirv;
    lvk::Result res = lvk::compileShader(vkShaderStageFromFileName(sourceFilename), shaderSource.c_str(), &spirv, glslang_default_resource());

    assert(!spirv.empty());

    saveSPIRVBinaryFile(destFilename, spirv.data(), spirv.size());
}

int main() {
    uint32_t w = 1280;
    uint32_t h = 800;

    // Init Log
    minilog::initialize("log_thread.html", { .htmlLog = true });

    tf::Taskflow taskflow;
    // tf::Task compileShadersTask = taskflow.emplace([]() {
    //     minilog::threadNameSet("Shader Compiler");
    //     minilog::callstackPushProc("std::thread->");
    //     glslang_initialize_process();
    //     testShaderCompilation("../data/main.vert", ".cache/Engine.vert.bin");
    //     testShaderCompilation("../data/main.frag", ".cache/Engine.frag.bin");
    //     glslang_finalize_process();
    //     minilog::callstackPopProc();
    // }).name("init");

    minilog::callstackPushProc("MainThread()->");

    GLFWwindow* window = initWindow("Daisuke Engine", w, h);

    glslang_initialize_process();
    testShaderCompilation("../data/main.vert", ".cache/Engine.vert.bin");
    testShaderCompilation("../data/main.frag", ".cache/Engine.frag.bin");
    glslang_finalize_process();

    {
        std::ofstream os("taskflow.dot");
        taskflow.dump(os);
    }

    tf::Executor executor;
    executor.run(taskflow).wait();

    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();
    }

    glfwDestroyWindow(window);
    glfwTerminate();

    minilog::callstackPopProc();
    minilog::deinitialize();
    return 0;
}