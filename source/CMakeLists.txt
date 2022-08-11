cmake_minimum_required (VERSION 3.12)

add_library (emutils SHARED
        ./net_utils.c
        ./proc_utils.c
)

set_target_properties(emutils PROPERTIES VERSION ${PROJECT_VERSION})
set_target_properties(emutils PROPERTIES SOVERSION 1)
set_target_properties(emutils PROPERTIES PUBLIC_HEADER ../include/libemutils.h)

configure_file(libemutils.pc.in libemutils.pc @ONLY)

target_compile_options (emutils PRIVATE -g3 -Wall -Werror -DCONFIG_BACKEND_FILE -DCONFIG_IEEE80211W -DCONFIG_DRIVER_WEXT -DCONFIG_WIRELESS_EXTENSION -DCONFIG_DRIVER_NL80211 -DCONFIG_LIBNL20 -DEAP_PSK -DIEEE8021X_EAPOL -DCONFIG_SHA256 -DCONFIG_CTRL_IFACE -DCONFIG_CTRL_IFACE_UNIX -DCONFIG_SME -DBUILD_LIB )
target_include_directories (emutils PRIVATE ../include ${CMAKE_SYSROOT}/usr/include ${H_FILES})
target_link_libraries (emutils PRIVATE c pthread rt)

install(TARGETS emutils
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
install(FILES ${CMAKE_BINARY_DIR}/libemutils.pc
    DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/pkgconfig)
