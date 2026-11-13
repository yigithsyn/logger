cmake_minimum_required(VERSION 3.10)

file(READ "${CMAKE_CURRENT_SOURCE_DIR}/VERSION" LOGGER_VERSION)
string(STRIP "${LOGGER_VERSION}" LOGGER_VERSION)

project(logger
    VERSION ${LOGGER_VERSION}
    DESCRIPTION "A small, dependency-free C logging library"
    LANGUAGES C
)


# project() initializes the prefix, so only replace CMake's default.
if(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
    if(WIN32 AND DEFINED ENV{LOCALAPPDATA} AND NOT "$ENV{LOCALAPPDATA}" STREQUAL "")
        file(TO_CMAKE_PATH "$ENV{LOCALAPPDATA}/Programs/logger" _default_install_prefix)
    elseif(UNIX AND DEFINED ENV{HOME} AND NOT "$ENV{HOME}" STREQUAL "")
        set(_default_install_prefix "$ENV{HOME}/.local")
    endif()
    if(DEFINED _default_install_prefix)
        set(CMAKE_INSTALL_PREFIX "${_default_install_prefix}" CACHE PATH
            "Install prefix for logger" FORCE)
        unset(_default_install_prefix)
    endif()
endif()

include(GNUInstallDirs)

add_library(logger src/logger.c)
add_library(logger::logger ALIAS logger)

target_include_directories(logger
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)
set_target_properties(logger PROPERTIES
    C_STANDARD 90
    C_STANDARD_REQUIRED YES
    C_EXTENSIONS NO
)

install(TARGETS logger
    EXPORT logger_targets
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
install(FILES include/logger.h DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
install(EXPORT logger_targets
    NAMESPACE logger::
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/logger
)

include(CTest)
if(BUILD_TESTING)
    add_executable(test_logger tests/test_logger.c)
    target_link_libraries(test_logger PRIVATE logger::logger)
    set_target_properties(test_logger PROPERTIES
        C_STANDARD 90
        C_STANDARD_REQUIRED YES
        C_EXTENSIONS NO
    )
    add_test(NAME logger COMMAND test_logger)
endif()
