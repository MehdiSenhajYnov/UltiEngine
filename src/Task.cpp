#include "Task.h"

Task::Task(const std::function<void()>& taskToDo) : toDo(taskToDo)
{
    
}

Task::Task(const std::function<void()>& taskToDo, Task* dependence) : toDo(taskToDo), Dependance(dependence)
{
}

void Task::Work()
{
    if (Dependance != nullptr && Dependance->Done)
    {
        Running = true;
        toDo();
        Done = true;
        Running = false;
    }
}
