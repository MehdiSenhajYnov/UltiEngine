#pragma once
#include <functional>
#include <map>
#include <mutex>
#include <vector>

#include "Task.h"

class Worker;

class WorkPool
{
public:
    WorkPool() = default;
    ~WorkPool();
    
    std::vector<Task>& GetListOfTasks();
    
    Task& AddTask(const std::function<void()>& taskToDo);
    Task& AddTaskWithDependences(const std::function<void()>& taskToDo, Task& dependence);
    
    void RemoveFirst();
    Task* GetTask();
    
    bool HasAvailableTasks();

    std::mutex& GetWorkMutex();
    std::condition_variable& GetWorkCV();

    void AddWorkers(int numbers);

    void Run();
    
    int GetNumberOfBusyWorkers();
    void IncrementNumberOfBusyWorkers();
    void DecrementNumberOfBusyWorkers();

    void Stop();
private:
    bool isRunning = true;
    
    std::vector<Task> listOfTasks;
    
    std::vector<Worker*> listOfWorkers;
    
    std::mutex TaskMutex;
    
    std::mutex numberOfBusyWorkerMutex;
    
    int numberOfBusyWorkers = 0;

    std::mutex workMutex;
    std::condition_variable workCV;
};
