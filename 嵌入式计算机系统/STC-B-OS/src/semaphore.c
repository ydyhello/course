#include "semaphore.h"

XDATA char semaphores[MAX_SEMAPHORES];
XDATA u8 proc_sem_wait_flag[MAX_SEMAPHORES];
XDATA u8 sem_wake_round[MAX_SEMAPHORES];
XDATA u8 proc_waiting_sem = 0;

XDATA char semaphore_lock[MAX_SEMAPHORES] = {0}; // ��ʼ��Ϊ0��ʾ����δ����״̬

void __sem_init(u8 sem_id, char val)
{
    proc_sem_wait_flag[sem_id] = 0;
    sem_wake_round[sem_id] = 1;
    semaphores[sem_id] = val;
}

void __sem_post(u8 sem_id)
{
    XDATA u8 select_wake;

    // ��ȡ��
    while (semaphore_lock[sem_id] != 0) {
        // �ȴ����ͷ�
    }
    semaphore_lock[sem_id] = 1; // ��ס

    // No process is waiting, only increment semaphore
    if (!proc_sem_wait_flag[sem_id]) {
        semaphores[sem_id]++;
        semaphore_lock[sem_id] = 0; // �ͷ���
        return;
    }

    // Find the first process waiting on the semaphore
    select_wake = RSL(sem_wake_round[sem_id]);
    while ((select_wake & proc_sem_wait_flag[sem_id]) == 0)
        select_wake = RSL(select_wake);

    // Remove the waiting flag of the process
    proc_sem_wait_flag[sem_id] &= ~select_wake;
    proc_waiting_sem &= ~select_wake;

    // Record the last woken process
    sem_wake_round[sem_id] = select_wake;

    // �ͷ���
    semaphore_lock[sem_id] = 0;
}

u8 __sem_wait(u8 sem_id)
{
    // ��ȡ��
    while (semaphore_lock[sem_id] != 0) {
        // �ȴ����ͷ�
    }
    semaphore_lock[sem_id] = 1; // ��ס

    // Semaphore is currently idle, decrement value and return.
    if (semaphores[sem_id]) {
        semaphores[sem_id]--;
        semaphore_lock[sem_id] = 0; // �ͷ���
        return 1;
    }

    // Set the current process as waiting on the semaphore, yield
    // (and the process won't be selected by the scheduler
    // until the wait flag is cleared by some sem_post)
    proc_sem_wait_flag[sem_id] |= BIT(current_process);
    proc_waiting_sem |= BIT(current_process);

    // �ͷ���
    semaphore_lock[sem_id] = 0;

    return 0; // Returning 0 means a reschedule should be done.
}
