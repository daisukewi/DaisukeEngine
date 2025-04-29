#include <iostream>
#include <cstdio>
#include <fstream>
#include <vector>

#include <shared/HelpersGLFW.h>
#include <taskflow/taskflow.hpp>
#include <taskflow/algorithm/for_each.hpp>

int main() {
    uint32_t w = 1280;
    uint32_t h = 800;

    GLFWwindow* window = initWindow("Daisuke Engine", w, h);

    tf::Taskflow taskflow;

    std::vector<int> items{ 1, 2, 3, 4, 5, 6, 7, 8 };

    auto task =
        taskflow.for_each_index(0u, static_cast<uint32_t>(items.size()), 1u, [&](int i) { printf("%i", items[i]); }).name("for_each_index");

    taskflow.emplace([]() { printf("\nS - Start\n"); }).name("S").precede(task);
    taskflow.emplace([]() { printf("\nT - End\n"); }).name("T").succeed(task);

    {
        std::ofstream os(".cache/taskflow.dot");
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