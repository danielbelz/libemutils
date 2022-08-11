/**
    @file libemutils.h
    @brief Definitions and prototypes for embedded utils library
*/

#ifndef LIB_EMUTILS_H
#define LIB_EMUTILS_H

#include <errno.h>

#define ERROR   -1
#define OK      0

// -----------------------------------------------------------------------------
// Network Utils Function prototypes
// -----------------------------------------------------------------------------

/**
    @brief Retrieves the IP Address for the given network interface

    @code

        const char* interface = "eth0";
        char ipaddr[16] = {0};

        if (getInterfaceIp(interface, ipaddr) == OK) {
            printf("%s IP address is %s\n", interface, ipaddr);
        } else {
            printf("Unable to get interface %s IP address\n", interface);
        }

    @endcode

    @param interface name of the network interface
    @param ipaddr (output) IP address of the interface

    @return OK if interface was found and IP address was retrieved
    @return ERROR if interface or IP address not available
*/
int getInterfaceIp(const char* interface, char *ipaddr);


// -----------------------------------------------------------------------------
// Process Utils Function prototypes
// -----------------------------------------------------------------------------

/**
    @brief Retrieves the PID of the given process/task name

    @code

        const char* process = "ps";
        pid_t pid = 0;

        pid = findProcessPid(process);
        if (pid > 0) {
            printf("%s PID is %d\n", process, pid);
        } else {
            printf("Unable to find %s PID\n", process);
        }

    @endcode

    @param task_name Name of the process

    @return 0 if task/process not found in procfs
    @return pid of the task if found (first occurrence)
*/
pid_t findProcessPid(char* task_name);

/**
    @brief Checks if the PID is exists

    @code

        const char* process = "ps";
        pid_t pid = 0;

        pid = findProcessPid(process);
        if (pid > 0) {
            printf("%s PID is %d\n", process, pid);
        } else {
            printf("Unable to find %s PID\n", process);
        }

        ...

        if (isPidUp(pid)) {
            printf("PID %d is UP\n", pid);
        } else {
            printf("PID %d seems to be down\n", pid);
        }

    @endcode

    @param pid PID to be checked

    @return true if PID exists
    @return false if PID doesn't exist or error
*/
bool isPidUp(pid_t pid);

/**
    @brief Kills all instances of the given process name

    @code

        const char* process = "top";
        killAllProcess(process);

    @endcode

    @param process_name Name of the process to be killed
*/
void killAllProcess(char *process_name);

#endif //LIB_EMUTILS_H