#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <assert.h>
#include <sys/types.h>
#include <sys/wait.h>

int main () {
    
    pid_t pid;
    
    switch (pid = fork()) {
            
        case -1:
            syserr("Error in fork\n");
            
        case 0: /* proces potomny */
            
            return 0;
            
        default: /* proces macierzysty */
            pid_t my_pid = getpid();
            assert(check_if_ancestor(my_pid, pid) == 1);
            assert(check_if_ancestor(my_pid, pid+1) == 0);
            assert(check_if_ancestor(my_pid-1, pid) == 0);
    }
    return 0;
}
