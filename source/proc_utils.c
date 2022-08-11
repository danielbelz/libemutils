
/**
    @file proc_utils.c
    Utilities to handle processes at system level

*/
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <stdbool.h>

#include <execinfo.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/prctl.h>

#define RESPAWN_WAIT 5

// TODO:
// Add a cleanup function/process
// Add arguments to execl
// Add way to disable respawn
void processMonitor(int respawn_wait_seconds, char *respawn_process)
{
	struct stat file_info = { 0 };
    bool do_respawn = false;

    if (respawn_process && stat(respawn_process, &file_info) == 0)
        do_respawn = true;

    // fork() returns 0 for child process, child-pid for parent process.
    if (fork() == 0) {
        int ppid = getppid();
        prctl(PR_SET_NAME, (unsigned long)"processMonitor", 0, 0, 0);
        printf("processMonitor started, watching %d:\n", ppid);

        while(1) {
            if (kill(ppid, 0) == -1) {
                if (do_respawn) {
                    printf("Application not responding. Scheduling respawn in %d seconds...\n", respawn_wait_seconds);
                } else {
                    printf("Application terminated. Won't respawn...\n");
                }
                break;
            } else {
                // Check every 5 seconds
                sleep(RESPAWN_WAIT);
            }
        }

        if (do_respawn) {
            printf("Respawning application...\n");
            execl(respawn_process, respawn_process, "start", NULL);
        }

        exit(0);
    }
}

//TODO:
// findPidFromName(char *process_name)
// isPidUp(pid_t pid)
// isProcessUp(char *process_name)
// killAllProcess(char *process_name)
