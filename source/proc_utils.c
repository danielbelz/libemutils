
/**
    @file proc_utils.c
    Utilities to handle processes at system level

*/
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <stdbool.h>
#include <dirent.h>

#include <execinfo.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/prctl.h>

#include "libemutils.h"

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

#define BUF_SIZE 1024
pid_t findProcessPid(char* task_name)
{
    pid_t pid = 0;
	DIR* dir;
	struct dirent* ptr;
	dir = opendir("/proc"); // Open the path to the

	if (NULL != dir) {
		while ((ptr = readdir(dir)) != NULL) // Loop reads each file/folder in the path
		{
            char filepath[50];      // The size is arbitrary, can hold the path of cmdline file
            char cur_task_name[50]; // The size is arbitrary, can hold to recognize the command line text
        	FILE* fp;

			// If it reads "." or ".." Skip, and skip the folder name if it is not read
			if ((strcmp(ptr->d_name, ".") == 0) || (strcmp(ptr->d_name, "..") == 0))
			    continue;

			if (DT_DIR != ptr->d_type)
				continue;

			sprintf(filepath, "/proc/%s/status", ptr->d_name); // Generates the path to the file to be read

			fp = fopen(filepath, "r");                         // Open the file
			if (NULL != fp) {
            	char buf[BUF_SIZE];
				if (fgets(buf, BUF_SIZE - 1, fp) == NULL) {
					fclose(fp);
					continue;
				}
				sscanf(buf, "%*s %s", cur_task_name);

				// Print the name of the path (that is, the PID of the process) if the file content meets the requirement
				if (!strcmp(task_name, cur_task_name)) {
                    pid = atoi(ptr->d_name);
                }
				fclose(fp);
			} else {
                fprintf(stderr, "%s: %s\n", __FUNCTION__, strerror(errno));
            }

            if (pid) {
                printf("%s pid %d\n", task_name, pid);
                // First PID entry found, return it
		        closedir(dir); // Shut down the path
                return pid;
            }

		}
		closedir(dir); // Shut down the path
	} else {
        fprintf(stderr, "%s: %s\n", __FUNCTION__, strerror(errno));
    }

    fprintf(stderr, "%s: %s not found\n", __FUNCTION__, task_name);
    return pid;
}

bool isPidUp(pid_t pid)
{
    bool ret = false;

    if (pid > 0 && kill(pid, 0) == 0) {
        ret = true;
    }

    return ret;
}

//TODO:
// isProcessUp(char *process_name)
// killAllProcess(char *process_name)
