#ifndef TASK_H
#define TASK_H

#include <functional>
#include <cstdint>
#include <pico/stdlib.h>

class Task {
public:
    using TaskFunction = std::function<void()>;

    Task(TaskFunction func, uint32_t interval)
        : func(func), interval(interval), lastRun(0) {}

    virtual ~Task() {}

    void operator() (uint32_t currentMillis) {
        if (currentMillis - lastRun >= interval) {
            func();
            lastRun = currentMillis;
        }
    }

    virtual void run() = 0;

private:
    TaskFunction func;
    uint32_t interval;
    uint32_t lastRun;
};

#endif // TASK_H
