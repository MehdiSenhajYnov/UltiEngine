#include "WorkPool.h"

#include <iostream>

#include "Worker.h"

WorkPool::~WorkPool()
{
    for (auto element : listOfWorkers)
        element->RequestStop();

    workCV.notify_all();
    
    for (auto element : listOfWorkers)
        delete element;
        
    listOfWorkers.clear();
}

std::vector<Task>& WorkPool::GetListOfTasks()
{
    return listOfTasks;
}

Task& WorkPool::AddTask(const std::function<void()>& taskToDo)
{
    std::unique_lock lock(TaskMutex);
    listOfTasks.push_back(Task(taskToDo));
    return listOfTasks.back();
}

Task& WorkPool::AddTaskWithDependences(const std::function<void()>& taskToDo, Task& dependence)
{
    std::unique_lock lock(TaskMutex);
    listOfTasks.push_back(Task(taskToDo, &dependence));
    return listOfTasks.back();
}

void WorkPool::RemoveFirst()
{
    std::unique_lock lock(TaskMutex);
    if (listOfTasks.empty())
        return;
    listOfTasks.erase(listOfTasks.begin());
}

bool WorkPool::HasAvailableTasks()
{
    return listOfTasks.size() > 0;
}

std::mutex& WorkPool::GetWorkMutex()
{
    return workMutex;
}

std::condition_variable& WorkPool::GetWorkCV()
{
    return workCV;
}

void WorkPool::AddWorkers(int numbers)
{
    for (int i = 0; i < numbers; i++)
    {
        auto newWorker = new Worker(this);
        listOfWorkers.push_back(newWorker);
        newWorker->Init();
    }
}

void WorkPool::Run()
{
    isRunning = true;
    
    char inputUser = 't';
    std::jthread userInputThread([&]
    {
        while (inputUser != 'q')
        {
            std::cin >> inputUser;
        }
        isRunning = false;
    });

    while (isRunning)
    {
        if (!GetListOfTasks().empty() && GetNumberOfBusyWorkers() < listOfWorkers.size())
        {
            workCV.notify_one();
        }
    }
}

int WorkPool::GetNumberOfBusyWorkers()
{
    std::unique_lock lock(numberOfBusyWorkerMutex);
    return numberOfBusyWorkers;
}

void WorkPool::IncrementNumberOfBusyWorkers()
{
    std::unique_lock lock(numberOfBusyWorkerMutex);
    numberOfBusyWorkers++;
}

void WorkPool::DecrementNumberOfBusyWorkers()
{
    std::unique_lock lock(numberOfBusyWorkerMutex);
    numberOfBusyWorkers--;
}

void WorkPool::Stop()
{
    isRunning = false;
}

Task* WorkPool::GetTask()
{
    std::unique_lock lock(TaskMutex);

    for (auto& task : listOfTasks)
    {
        if (task.Dependance != nullptr && !task.Dependance->Done)
        {
            continue;
        }
        listOfTasks.erase(listOfTasks.begin());
        return &task;
    }
    return nullptr;
}
