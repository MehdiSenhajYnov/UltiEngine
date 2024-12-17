#pragma once
#include <functional>
#include <thread>


class Task;
class WorkPool;

class Worker
{
public:
    Worker(WorkPool* _workpool);
    
    void Init();
    bool isRunning = false;
    const std::function<void()>* GetCuurentWork();
    void SetCuurentWork(std::function<void()>* newWork);

    void RequestStop();

private:
    WorkPool* workpool;
    const std::function<void()>* currentWork = nullptr;
    void Work(const Task& taskToDo);

    std::jthread thread;
};
