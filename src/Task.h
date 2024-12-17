#pragma once
#include <functional>

class Task
{
public:
    Task(const std::function<void()>& taskToDo);
    Task(const std::function<void()>& taskToDo, Task* dependence);
    
    void Work();
    std::function<void()> toDo;
    bool Done = false;
    bool Running = false;
    Task* Dependance = nullptr;
};
