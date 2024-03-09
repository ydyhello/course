#include "stack.h"

IDATA u8 kernel_stack[KERNEL_STACKSIZE];
IDATA u8 process_stack[5][PROCESS_STACKSIZE];
XDATA u8 process_stack_swap[3][PROCESS_STACKSIZE];

/*
		OS allows 8 processes maximum, but there are only
		5 process stacks allowed in memory due to size 
		limitations.
		
		the process stacks can be swapped out to xdata 
		swap area if there are more than 5 processes at
		the same time.
*/

char get_stack_index(u8 pid)
{
	XDATA u8 i;
	for(i=0;i<5;i++)
		if(process_stack[i][PROCESS_STACKSIZE-1] == pid)
			return i;
	
	return -1;
}

char get_stackswap_index(u8 pid)
{
	XDATA u8 i;
	for(i=0;i<3;i++)
		if(process_stack_swap[i][PROCESS_STACKSIZE-1] == pid)
			return i;
	
	return -1;
}


extern XDATA u8 process_context[8][18];
extern XDATA u8 process_slot;
/*
void stackswap(u8 swap_index)
{
	XDATA u8 i, temp, physical_index;
	
	//first try to find a stack slot that's not being used
	physical_index = 0xff;
	for(i=0;i<5;i++)
	{
		if(process_slot & BIT(process_stack[i][PROCESS_STACKSIZE-1]) == 0)
		{
			physical_index = i;
			break;
		}
	}
	
	//if there's no free stack slot, select random victim
	physical_index = rand32()%5;
	
	for(i=0; i<PROCESS_STACKSIZE; i++)
	{
		temp = process_stack[physical_index][i];
		process_stack[physical_index][i] = process_stack_swap[swap_index][i];
		process_stack_swap[swap_index][i] = temp;
	}
	
	//convert context SP of process whose stack is being swapped out to a relative address
	process_context[process_stack_swap[swap_index][PROCESS_STACKSIZE-1]][17] -= (u8)process_stack[physical_index];
	
	//convert context SP of process whose stack is being swapped in to an absolute address
  process_context[process_stack[physical_index][PROCESS_STACKSIZE-1]][17] += (u8)process_stack[physical_index];
}
*/
void stackswap(u8 swap_index)
{
    XDATA u8 i, temp, least_recently_used;

    // Find the least recently used stack slot
    least_recently_used = 0; // 跟踪最近最少使用的栈槽位的索引

	//从索引1开始，查找最近最少使用的栈槽位。这是通过比较栈槽位的最后一个元素，即进程ID，来实现的。最小的进程ID表示最近最少使用的栈。
    for (i = 1; i < 5; i++) {
        if (process_stack[i][PROCESS_STACKSIZE-1] < process_stack[least_recently_used][PROCESS_STACKSIZE-1]) {
            least_recently_used = i;
        }
    }

    for (i = 0; i < PROCESS_STACKSIZE; i++) {
        temp = process_stack[least_recently_used][i];
        process_stack[least_recently_used][i] = process_stack_swap[swap_index][i];
        process_stack_swap[swap_index][i] = temp;
    }

    // Convert context SP of process whose stack is being swapped out to a relative address
    process_context[process_stack_swap[swap_index][PROCESS_STACKSIZE-1]][17] -= (u8)process_stack[least_recently_used];

    // Convert context SP of process whose stack is being swapped in to an absolute address
    process_context[process_stack[least_recently_used][PROCESS_STACKSIZE-1]][17] += (u8)process_stack[least_recently_used];
}

