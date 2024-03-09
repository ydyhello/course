#ifndef _MUTEX_H_
#define _MUTEX_H_

#include "global.h"

// ���廥�������ݽṹ
typedef struct {
    XDATA char locked;
} Mutex;

// ��ʼ��������
void mutex_init(Mutex* mutex);

// ����������
void mutex_lock(Mutex* mutex);

// ����������
void mutex_unlock(Mutex* mutex);

#endif
