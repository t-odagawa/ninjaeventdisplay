################ Generic settings ################

# Be verbose
# set(CMAKE_VERBOSE_MAKEFILE ON)

if (${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_BINARY_DIR})
    message(FATAL_ERROR "DO NOT BUILD in-tree.")
endif ()

# CMAKE 3.10.2 is the default version in the Ubuntu 18.04 repositories
cmake_minimum_required(VERSION 3.10.2)

# Name of the project
project(DrawNinjaSf
        DESCRIPTION "Draw event displays from .ninjasf files"
        LANGUAGES CXX)

# Summary of Build types :
# RelWithDebInfo [-O2 –g] (default)
# Debug [-g -O0 -ggdb]
# Release [-O3]

# Build type
if (NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
    message(STATUS "Setting build type to 'Release' as none was specified.")
    set(CMAKE_BUILD_TYPE Release CACHE STRING "Choose the type of build." FORCE)
    # Set the possible values of build type for cmake-gui
    set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug" "Release"
            "MinSizeRel" "RelWithDebInfo")
endif ()

# The version number
set(PROJECT_VERSION_MAJOR 0)
set(PROJECT_VERSION_MINOR 0)
set(PROJECT_VERSION_PATCH 1)

set(PROJECT_VERSION "${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}.${PROJECT_VERSION_PATCH}")
add_compile_definitions(VERSION=${PROJECT_VERSION})

# Only UNIX OSes
if (NOT UNIX)
    message(FATAL_ERROR "Non UNIX OSes are not supported")
endif ()

# Additional include directories for MacOS
if (APPLE)
    include_directories(/usr/local/include)
    include_directories(/opt/local/include)
    link_directories(/usr/local/lib)
    link_directories(/opt/local/lib)
endif ()

# Set default install path to the same path of the source code
if (CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
    set(CMAKE_INSTALL_PREFIX "${CMAKE_SOURCE_DIR}" CACHE PATH "default install path" FORCE)
endif ()

# Set install paths for binaries, libraries and headers
set(CMAKE_INSTALL_BINDIR ${CMAKE_INSTALL_PREFIX}/bin)

if (${TEXT_MODE})
   add_Definitions(-DTEXT_MODE=ON)
endif()

################ Find packages ################

# where to look first for cmake modules
set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake/modules")

# boost program_options, system, filesystem and log are required
set(Boost_USE_STATIC_LIBS OFF)
find_package(Boost 1.73 COMPONENTS system filesystem program_options unit_test_framework log_setup log REQUIRED)
if (Boost_FOUND)
    message(STATUS "Boost library -- found")
    message(STATUS "Boost library include dir : ${Boost_INCLUDE_DIRS}")
    include_directories("${Boost_INCLUDE_DIRS}")
endif()

################ Libraries ################

set(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)
set(NINJA_SF_EVEDIS_LIBRARIES "")
if (NOT APPLE)
    list(APPEND NINJA_SF_EVEDIS_LIBRARIES
            ${Boost_LIBRARIES}
            )
else ()
    list(APPEND NINJA_SF_EVEDIS_LIBRARIES
            Boost::program_options
            Boost::system
            Boost::filesystem
            Boost::log_setup
            Boost::log
            )
endif ()

################ Compiler flags ################

# Set C++11 standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_PLATFORM_INDEPENDENT_CODE ON)
add_definitions(-DBOOST_TEST_DYN_LINK)
add_definitions(-DBOOST_LOG_DYN_LINK)

message(STATUS "Compiler ID : ${CMAKE_CXX_COMPILER_ID}")

if (CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -pedantic -Wno-overloaded-virtual -Wno-shadow")
elseif (CMAKE_COMPILER_IS_GNUCC OR CMAKE_COMPILER_IS_GNUCXX)
    # Add -O0 to remove optimizations when using gcc and debugging
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -O0 -ggdb -U_FORTIFY_SOURCE -D_FORTIFY_SOURCE=0")
    set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -O0 -ggdb -U_FORTIFY_SOURCE -D_FORTIFY_SOURCE=0")
    # show all warnings -Wall
    # be pedantic -pedantic
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -pedantic -Wno-overloaded-virtual -Wno-shadow \
     -Wno-vla -Wno-unused-parameter -Wno-unused-but-set-variable -Wno-sign-compare -Wno-deprecated-copy \
     -Wno-unused-variable -Wno-return-type -Wno-comment")
endif ()

################ Linker flags ################

set(CMAKE_INSTALL_PPATH "${CMAKE_INSTALL_LIBDIR}")
set(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)
set(NINJA_SF_EVEDIS_LIBRARIES ${Boost_LIBRARIES})

################ Sub Directories ################

add_subdirectory(src)