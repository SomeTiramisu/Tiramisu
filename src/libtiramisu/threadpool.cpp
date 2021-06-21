#include "threadpool.h"

ThreadPool::ThreadPool()
{
    m_threads.resize(m_maxThread);
    m_freeThreads.assign(m_maxThread, false);
}

std::thread ThreadPool::getThread() {

}
