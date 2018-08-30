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
            return 0;
            
        case 0: /* proces potomny */
            
            return 0;
            
        default: /* proces macierzysty */
            printf("%d\n", check_if_ancestor(getpid(), pid));
            printf("%d\n", check_if_ancestor(getpid()+1, pid));
            printf("%d\n", check_if_ancestor(getpid()-1, pid));
    }
    return 0;
}
