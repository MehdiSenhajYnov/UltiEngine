#include <functional>
#include <iostream>
#include <vector>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "SomeClass.h"
#include "Worker.h"

#include <thread>
#include <chrono>

#include "WorkPool.h"

#pragma comment(lib, "OpenGL32.lib")
#pragma comment(lib, "glfw3.lib")

const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

std::mutex mtx;
void LockedPrint(std::string msg)
{
    std::unique_lock lock(mtx);
    std::cout << msg << std::endl;
}

void MaFunction()
{
    LockedPrint("MaFunction");
}

void TpWorkers()
{
    WorkPool workPool;

    workPool.AddWorkers(5);
    Task* addedtask = nullptr;
    for (int i = 0; i < 50; i++)
    {
        if (addedtask == nullptr)
        {
            addedtask = &workPool.AddTask([i]
            {
                LockedPrint("task : " + std::to_string(i));
            });
        } else if (i % 2 == 0 )
        {
            addedtask = &workPool.AddTaskWithDependences([i]
            {
                LockedPrint("task : " + std::to_string(i));
            },*addedtask);
        }
        else
        {
            addedtask = &workPool.AddTask([i]
            {
                LockedPrint("task : " + std::to_string(i));
            });
        }
    }
    
    workPool.AddTaskWithDependences([] {     MaFunction(); },*addedtask);

    workPool.Run();
}



void processInput(GLFWwindow *window)
{
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}





int main(int argc, char* argv[])
{
    // glfwInit();
    // glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    // glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    // glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    //
    // GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "UltiEngine", nullptr, nullptr);
    // if (window == nullptr)
    // {
    //     std::cout << "Failed to create GLFW window" << '\n';
    //     glfwTerminate();
    //     return -1;
    // }
    // glfwMakeContextCurrent(window);
    // glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    //
    // if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    // {
    //     std::cout << "Failed to initialize GLAD" << '\n';
    //     return -1;
    // }  
    //
    // while (!glfwWindowShouldClose(window))
    // {
    //     processInput(window);
    //
    //     glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    //     glClear(GL_COLOR_BUFFER_BIT);
    //
    //     glfwSwapBuffers(window);
    //     glfwPollEvents();
    // }
    //
    //
    // glfwTerminate();
    //
    //
    TpWorkers();
    
    return 0;
}

