#ifndef THREADPOOL_H
#define THREADPOOL_H

#include <thread>
#include <vector>
#include <queue>
#include <functional>
#include "utils/utils.h"

using ThFunction = std::function<void()>;

class ThreadPool
{
public:
    ThreadPool();
    void start(const ThFunction& f); //take the ownership
    void clear();
    void waitForDone();
private:
    inline ThFunction makeFunction(const ThFunction& f, int i);
    std::vector<std::thread> m_threads;
    std::queue<int> m_threadsReady;
    std::queue<ThFunction> m_toRun;
};

#endif // THREADPOOL_H
