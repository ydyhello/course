#ifndef _MUTEX_H_
#define _MUTEX_H_

#include "global.h"

// 定义互斥锁数据结构
typedef struct {
    XDATA char locked;
} Mutex;

// 初始化互斥锁
void mutex_init(Mutex* mutex);

// 锁定互斥锁
void mutex_lock(Mutex* mutex);

// 解锁互斥锁
void mutex_unlock(Mutex* mutex);

#endif
