/**
    @file libemutils.h
    @brief Definitions and prototypes for embedded utils library
*/

#ifndef LIB_EMUTILS_H
#define LIB_EMUTILS_H

#define ERROR   -1
#define OK      0

// -----------------------------------------------------------------------------
// Function prototypes
// -----------------------------------------------------------------------------

/**
    @brief Retrieves the IP Address from the given network interface

    @code


    @endcode

    @param interface
    @param ipaddr

    @return OK if interface was found and IP address was retrieved
    @return ERROR if interface or IP address not available
 */
int getInterfaceIp(const char* interface, char *ipaddr);


#endif //LIB_EMUTILS_H