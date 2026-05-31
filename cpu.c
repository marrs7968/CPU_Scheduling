#include <stdio.h>
#include <limits.h>
#include "oslabs.h"

// ============================ PRIORITY-BASED PREEMPTIVE SCHEDULING ALGORITHM ===================
struct PCB handle_process_arrival_pp(struct PCB ready_queue[QUEUEMAX], int *queue_cnt, struct PCB current_process, struct PCB new_process, int timestamp) {
    if (current_process.process_id == 0 && current_process.arrival_timestamp == 0 && current_process.total_bursttime == 0 && 
        current_process.execution_starttime == 0 && current_process.execution_endtime == 0 && current_process.remaining_bursttime == 0 && 
        current_process.process_priority == 0) {
        // When there is no current process, new process is set to current timestamp
        new_process.execution_starttime = timestamp;
        new_process.execution_endtime = timestamp + new_process.total_bursttime;
        new_process.remaining_bursttime = new_process.total_bursttime;

        return new_process;
    } else {
        if (new_process.process_priority > current_process.process_priority) {
            new_process.remaining_bursttime = new_process.total_bursttime;
            new_process.execution_starttime = 0;
            new_process.execution_endtime = 0;
            ready_queue[*queue_cnt] = new_process;
            (*queue_cnt)++;

            return current_process;
        } else {
            current_process.execution_endtime = 0;
            current_process.remaining_bursttime = current_process.total_bursttime - (timestamp - current_process.execution_starttime);
            ready_queue[*queue_cnt] = current_process;
            new_process.execution_starttime = timestamp;
            new_process.execution_endtime = timestamp + new_process.total_bursttime;
            new_process.remaining_bursttime = new_process.total_bursttime;
            (*queue_cnt)++;
            
            return new_process;
        }
    }
}
struct PCB handle_process_completion_pp(struct PCB ready_queue[QUEUEMAX], int *queue_cnt, int timestamp) {
    if (*queue_cnt == 0) {
        return (struct PCB){0, 0, 0, 0, 0, 0, 0}; // No process in the ready queue
    } else {
        int max_priority = INT_MAX;
        int max_index = -1;
        for (int i = 0; i < *queue_cnt; i++) {
            if (ready_queue[i].process_priority < max_priority) {
                max_priority = ready_queue[i].process_priority;
                max_index = i;
            }
        }
        // Remove the process from the ready queue
        ready_queue[max_index].execution_starttime = timestamp;
        ready_queue[max_index].execution_endtime = timestamp + ready_queue[max_index].remaining_bursttime;
        struct PCB next_process = ready_queue[max_index];
        
        for (int i = max_index; i < *queue_cnt - 1; i++) {
            ready_queue[i] = ready_queue[i + 1];
        }
        (*queue_cnt)--;

        return next_process;
    }
}

// ============================ SHORTEST REMAINING TIME NEXT SCHEDULING ALGORITHM =================================
struct PCB handle_process_arrival_srtp(struct PCB ready_queue[QUEUEMAX], int *queue_cnt, struct PCB current_process, struct PCB new_process, int time_stamp) {
    if (current_process.process_id == 0 && current_process.arrival_timestamp == 0 && current_process.total_bursttime == 0 && 
        current_process.execution_starttime == 0 && current_process.execution_endtime == 0 && current_process.remaining_bursttime == 0 && 
        current_process.process_priority == 0) {
        // When there is no current process, new process is set to current timestamp
        new_process.execution_starttime = time_stamp;
        new_process.execution_endtime = time_stamp + new_process.total_bursttime;
        new_process.remaining_bursttime = new_process.total_bursttime;

        return new_process;
    } else {
        if (current_process.remaining_bursttime < new_process.total_bursttime) {
            new_process.execution_starttime = 0;
            new_process.execution_endtime = 0;
            new_process.remaining_bursttime = new_process.total_bursttime;
            ready_queue[*queue_cnt] = new_process;
            (*queue_cnt)++;

            return current_process;
        } else {
            new_process.execution_starttime = time_stamp;   
            new_process.execution_endtime = time_stamp + new_process.total_bursttime;
            new_process.remaining_bursttime = new_process.total_bursttime;
            current_process.remaining_bursttime = current_process.total_bursttime - (time_stamp - current_process.execution_starttime);
            current_process.execution_starttime = 0;
            current_process.execution_endtime = 0;
            ready_queue[*queue_cnt] = current_process;
            (*queue_cnt)++;
            
            return new_process;
        }
    }
}

struct PCB handle_process_completion_srtp(struct PCB ready_queue[QUEUEMAX], int *queue_cnt, int timestamp) {
    if (*queue_cnt == 0) {
        return (struct PCB){0, 0, 0, 0, 0, 0, 0}; // No process in the ready queue
    } else {
        int min_burst = INT_MAX;
        int min_index = -1;
        for (int i = 0; i < *queue_cnt; i++) {
            if (ready_queue[i].remaining_bursttime < min_burst) {
                min_burst = ready_queue[i].remaining_bursttime;
                min_index = i;
            }
        }
        // Remove the process from the ready queue
        ready_queue[min_index].execution_starttime = timestamp;
        ready_queue[min_index].execution_endtime = timestamp + ready_queue[min_index].remaining_bursttime;
        struct PCB next_process = ready_queue[min_index];
        
        for (int i = min_index; i < *queue_cnt - 1; i++) {
            ready_queue[i] = ready_queue[i + 1];
        }
        (*queue_cnt)--;

        return next_process;
    }
}

// ============================ ROUND ROBIN SCHEDULING ALGORITHM =================================
struct PCB handle_process_arrival_rr(struct PCB ready_queue[QUEUEMAX], int *queue_cnt, struct PCB current_process, struct PCB new_process, int timestamp, int time_quantum) {
    if (current_process.process_id == 0 && current_process.arrival_timestamp == 0 && current_process.total_bursttime == 0 && 
        current_process.execution_starttime == 0 && current_process.execution_endtime == 0 && current_process.remaining_bursttime == 0 && 
        current_process.process_priority == 0) {
        // When there is no current process, new process is set to current timestamp
        new_process.execution_starttime = timestamp;
        // Calculating execution end time based on the smaller of time quantum and total burst time
        if (time_quantum > new_process.total_bursttime) {
            new_process.execution_endtime = timestamp + new_process.total_bursttime;
        } else {
            new_process.execution_endtime = timestamp + time_quantum;
        }
        // Remaining burst time is set to total burst time
        new_process.remaining_bursttime = new_process.total_bursttime;

        return new_process;
    } else {
        new_process.remaining_bursttime = new_process.total_bursttime;
        new_process.execution_starttime = 0;
        new_process.execution_endtime = 0;
        ready_queue[*queue_cnt] = new_process;
        (*queue_cnt)++;
        return current_process;
    }
}
struct PCB handle_process_completion_rr(struct PCB ready_queue[QUEUEMAX], int *queue_cnt, int timestamp, int time_quantum) {
    if (*queue_cnt == 0) {
        return (struct PCB){0, 0, 0, 0, 0, 0, 0}; // No process in the ready queue
    } else {
        int min_arrival = INT_MAX;
        int min_index = -1;
        for (int i = 0; i < *queue_cnt; i++) {
            if (ready_queue[i].arrival_timestamp < min_arrival) {
                min_arrival = ready_queue[i].arrival_timestamp;
                min_index = i;
            }
        }
        // Remove the process from the ready queue
        ready_queue[min_index].execution_starttime = timestamp;
        if (time_quantum > ready_queue[min_index].remaining_bursttime) {
            ready_queue[min_index].execution_endtime = timestamp + ready_queue[min_index].remaining_bursttime;
        } else {
            ready_queue[min_index].execution_endtime = timestamp + time_quantum;
        }
        struct PCB next_process = ready_queue[min_index];
        
        for (int i = min_index; i < *queue_cnt - 1; i++) {
            ready_queue[i] = ready_queue[i + 1];
        }
        (*queue_cnt)--;

        return next_process;
    }
}
