#ifndef _H_LOCKER_H_
#define _H_LOCKER_H_

#include <exception>
#include <pthread.h>
#include <semaphore.h>

/**
 * @brief 封装的信号量的类
 * 
 */
class Sem {
  private:
    sem_t sem_;

  public:
    Sem() {
        if (sem_init(&sem_, 0, 0) != 0) {
            throw std::exception();
        }
    }

    ~Sem() {
        sem_destroy(&sem_);
    }

    bool Wait() {
        return sem_wait(&sem_) == 0;
    }

    bool Post() {
        return sem_post(&sem_) == 0;
    }
};

/**
 * @brief 互斥锁的封装
 * 
 */
class Locker {
  private:
    pthread_mutex_t mutex_;

  public:
    Locker() {
        if (pthread_mutex_init(&mutex_, NULL) != 0) {
            throw std::exception();
        }
    }

    ~Locker() {
        pthread_mutex_destroy(&mutex_);
    }

    bool Lock() {
        return pthread_mutex_lock(&mutex_) == 0;
    }

    bool Unlock() {
        return pthread_mutex_unlock(&mutex_) == 0;
    }
};

/**
 * @brief 条件变量
 *
 */
class Cond {
  private:
    pthread_mutex_t mutex_;
    pthread_cond_t  cond_;

  public:
    Cond() {
        if (pthread_mutex_init(&mutex_, NULL) != 0) {
            throw std::exception();
        }

        if (pthread_cond_init(&cond_, NULL) != 0) {
            pthread_mutex_destroy(&mutex_);
            throw std::exception();
        }
    }

    ~Cond() {
        pthread_mutex_destroy(&mutex_);
        pthread_cond_destroy(&cond_);
    }

    bool Wait() {
        int ret = 0;
        pthread_mutex_lock(&mutex_);
        ret = pthread_cond_wait(&cond_, &mutex_);
        pthread_mutex_unlock(&mutex_);
        return ret == 0;
    }

    /**
     * @brief 唤醒
     *
     * @return true
     * @return false
     */
    bool Signal() {
        return pthread_cond_signal(&cond_) == 0;
    }
};

#endif //_H_LOCKER_H_