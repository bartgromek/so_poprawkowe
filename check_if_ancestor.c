#include <lib.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <minix/rs.h>

int get_pm_endpt(endpoint_t *pt)
{
    return minix_rs_lookup("pm", pt);
}

uint8_t check_if_ancestor(pid_t pid1, pid_t pid2)
{
    printf("no siema\n");
    endpoint_t pm_pt;
    message m;
    m.m1_i1 = (int)pid1;
    m.m1_i2 = (int)pid2;

    if (get_pm_endpt(&pm_pt) != 0)
    {
        errno = ENOSYS;
        return -1;
    }
    printf("przed: %d\n", m.m1_i1);
    _syscall(pm_pt, PM_CHECKIFANCESTOR, &m);
    printf("po: %d\n", m.m1_i1);
    return m.m1_i1;
}
