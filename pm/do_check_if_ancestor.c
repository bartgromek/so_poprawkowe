#include <lib.h>
#include <stdio.h>
#include <string.h>
#include "pm.h"
#include "mproc.h"
#include "proto.h"

int find_index(pid_t pid) {
    int i;
    for (i = 0; i < NR_PROCS; i++) {
        if (mproc[i].mp_pid == pid) return i;
    }
    return -1;
}

int do_check_if_ancestor_recursion(pid_t pid1, pid_t pid2) {
    int pid2_idx, pid2_parent_idx;
    pid_t pid2_parent;
    if ( (pid2_idx = find_index(pid2)) == -1) return 0; // check if process with pid2 exists
    pid2_parent_idx = mproc[pid2_idx].mp_parent;
    if ( (pid2_parent = mproc[pid2_parent_idx].mp_pid) == 0) return 0; // we have reached root
    if (pid2_parent == pid1) return 1; // pid1 is an ancestor of pid2
    return check_if_ancestor(pid1, pid2_parent);
}

int do_check_if_ancestor() {
    pid_t pid1 = m_in.m1_i1, pid2 = m_in.m1_i2;
    return do_check_if_ancestor_recursion(pid1, pid2);
}


