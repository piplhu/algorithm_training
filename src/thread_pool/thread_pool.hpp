/**
 * @file thread_pool.hpp
 * @author pipl (huhui@hupipl.cn)
 * @brief 半同步/半反应堆线程池
 * @version 0.1
 * @date 2020-07-20
 *
 * @copyright Copyright (c) 2020
 *
 */
#ifndef _H_THREAD_POOL_H_
#define _H_THREAD_POOL_H_

#include <exception>
#include <iostream>
#include <list>
#include <pthread.h>

#include "locker.h"

template <typename T> class ThreadPool {
  private:
    int            thread_num_;   //线程池的线程数
    int            max_request_;  //请求队列中的最大请求数
    pthread_t *    threads_;      //描述线程池的数组
    std::list<T *> work_queue_;   //请求队列
    Locker         queue_locker_; //队列互斥锁
    Sem            queue_stat_;   //是否有任务需要处理
    bool           stop_;         //是否结束线程
  public:
    /**
     * @brief Construct a new Thread Pool object
     *
     * @param threadNum
     * @param maxRequeue
     */
    ThreadPool(int threadNum = 8, int maxRequeue = 10000);
    ~ThreadPool();

    /**
     * @brief 添加任务
     *
     * @param request
     * @return true
     * @return false
     */
    bool Append(T *request);

  private:
    /**
     * @brief 工作线程运行的函数，不断从队列中取任务执行
     *
     * @param arg
     */
    static void *Worker(void *arg);
    void         Run();
};

template <typename T>
ThreadPool<T>::ThreadPool(int threadNum, int maxRequeue)
    : thread_num_(threadNum)
    , max_request_(maxRequeue)
    , stop_(false)
    , threads_(NULL) {
    if (threadNum <= 0 || maxRequeue <= 0) {
        throw std::exception();
    }

    threads_ = new pthread_t[threadNum];
    if (!threads_) {
        throw std::exception();
    }

    //创建线程，并将它们都设置为脱离线程
    for (int i = 0; i < threadNum; i++) {
        std::cout << "create the" << i << "th thread" << std::endl;
        if (pthread_create(threads_ + i, NULL, Worker, this) != 0) {
            delete[] threads_;
            throw std::exception();
        }
        if (pthread_detach(threads_[i])) {
            delete[] threads_;
            throw std::exception();
        }
    }
}

template <typename T> ThreadPool<T>::~ThreadPool() {
    delete[] threads_;
    stop_ = true;
}

template <typename T> bool ThreadPool<T>::Append(T *request) {
    queue_locker_.Lock();
    if (work_queue_.size() > max_request_) {
        queue_locker_.Unlock();
        return false;
    }

    work_queue_.push_back(request);
    queue_locker_.Unlock();
    queue_stat_.Post();
    return true;
}

template <typename T> void *ThreadPool<T>::Worker(void *arg) {
    ThreadPool *pool = (ThreadPool *)arg;
    pool->Run();
    return pool;
}

template <typename T> void ThreadPool<T>::Run() {
    while (!stop_) {
        queue_stat_.Wait();
        queue_locker_.Lock();
        if (work_queue_.empty()) {
            queue_locker_.Unlock();
            continue;
        }
        T *request = work_queue_.front();
        work_queue_.pop_front();
        queue_locker_.Unlock();
        if (!request) continue;
        request->process();
        std::cout << "current thread id:" << pthread_self() << std::endl;
    }
}
#endif //_H_THREAD POOL_H_      