#ifndef THREADPOOL_H
#define THREADPOOL_H

#include <thread>
#include <vector>

class ThreadPool
{
public:
    ThreadPool();
    std::thread getThread();
private:
    unsigned int m_maxThread = std::thread::hardware_concurrency();
    std::vector<std::thread> m_threads;
    std::vector<bool> m_freeThreads;
};

#endif // THREADPOOL_H
