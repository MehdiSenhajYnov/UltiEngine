#include "Worker.h"
#include "WorkPool.h"

Worker::Worker(WorkPool* _workpool) : workpool(_workpool)
{
}

void Worker::Init()
{
    thread = std::jthread([this] { 
        isRunning = true;
        while (isRunning)
        {
            std::unique_lock lock(workpool->GetWorkMutex());
            workpool->GetWorkCV().wait(lock, [this] { return workpool->HasAvailableTasks() || !isRunning; });

            if (!isRunning)
                break;
            
            Task* taskToDo = workpool->GetTask();
            if (taskToDo == nullptr)
            {
                continue;
            }
            Work(*taskToDo);
        }
    });

}

const std::function<void()>* Worker::GetCuurentWork()
{
    return currentWork;
}

void Worker::SetCuurentWork(std::function<void()>* newWork)
{
    currentWork = newWork;
}

void Worker::Work(const Task& taskToDo)
{
    workpool->IncrementNumberOfBusyWorkers();
    currentWork = &taskToDo.toDo;

    if (taskToDo.toDo)
        taskToDo.toDo();
    
    currentWork = nullptr;
    workpool->DecrementNumberOfBusyWorkers();
}

void Worker::RequestStop()
{
    isRunning = false; 
}
