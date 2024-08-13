#ifndef SCHEDULER_H
#define SCHEDULER_H

#include "task.h"
#include <vector>
#include <cstdint>
#include <algorithm>
#include <pico/stdlib.h>

class Scheduler {
public:
    void addTask(Task* task) {
        tasks.push_back(task);
    }

    void run() {
        uint32_t currentMillis = millis();
        for (auto& task : tasks) {
            (*task)(currentMillis);
        }
    }

private:
    std::vector<Task*> tasks;

    uint32_t millis() {
        return to_ms_since_boot(get_absolute_time());
    }
};

#endif // SCHEDULER_H
