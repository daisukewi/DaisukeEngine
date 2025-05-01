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

    if (!f)
        return;

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

    GLFWwindow* window = initWindow("Daisuke Engine", w, h);

    tf::Taskflow taskflow;

    tf::Task shadersTask = taskflow.emplace([] (tf::Subflow& subflow) {
        tf::Task B1 = subflow.emplace([](){testShaderCompilation("../data/main.vert", ".cache/Engine.vert.bin");}).name("vertices");
        tf::Task B2 = subflow.emplace([](){testShaderCompilation("../data/main.frag", ".cache/Engine.frag.bin");}).name("fragments");
        tf::Task B3 = subflow.emplace([](){}).name("geometry");
    }).name("Shaders");

    tf::Task init = taskflow.emplace([](){ glslang_initialize_process(); }).name("init").precede(shadersTask);;
    tf::Task stop = taskflow.emplace([](){ glslang_finalize_process(); }).name("stop").succeed(shadersTask);

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

    return 0;
}