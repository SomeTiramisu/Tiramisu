#include "threadpool.h"

ThreadPool::ThreadPool() {
    m_threads.resize(std::thread::hardware_concurrency());
    for(size_t i=0; i<m_threads.size(); i++) {
        m_threadsReady.push(i);
    }
}

void ThreadPool::start(const ThFunction& f) {
    if(m_threadsReady.empty()) {
        m_toRun.push(std::move(f));
    } else {
        int i = m_threadsReady.front();
        m_threadsReady.pop();
        m_threads.at(i) = std::thread(makeFunction(f, i));
    }
}


void ThreadPool::clear() {
    m_toRun = std::queue<ThFunction>();
}

void ThreadPool::waitForDone() {
    for (std::vector<std::thread>::iterator it = m_threads.begin(); it < m_threads.end(); it++) {
        if(it->joinable()) {
            it->join();
        }
    }
}

ThFunction ThreadPool::makeFunction(const ThFunction &f, int i) {
    return [this, f, i]{
        f();
        while(not this->m_toRun.empty()) {
            ThFunction f2 = this->makeFunction(m_toRun.front(), i);
            m_toRun.pop();
            f2();
        }
        m_threadsReady.push(i);
    };
}
