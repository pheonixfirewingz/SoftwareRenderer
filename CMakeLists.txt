cmake_minimum_required(VERSION 3.22.1)
project(software_renderer C CXX)
#options
set(TERMIANL_MODE ON)
#options end

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake")

include(FlagControl)

# Debug mode by default.
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug)
endif()

if(CMAKE_BUILD_TYPE MATCHES Debug)
    add_definitions(-DWITH_DEBUG)
endif()

if(${TERMIANL_MODE})
    add_definitions(-DTERMIANL_MODE)
endif()

add_definitions(-DROOT_PATH="${PROJECT_SOURCE_DIR}")

if(CMAKE_SYSTEM_NAME MATCHES Linux)
    message(INFO "[SOFTRENDER] - building for Linux....")
    add_definitions(-DON_LINUX)
    set(ON_LINUX ON)
    set(CMAKE_CXX_STANDARD 23)
    set(CMAKE_CXX_STANDARD_REQUIRED ON)
elseif(CMAKE_SYSTEM_NAME MATCHES WindowsStore)
    message(INFO "[SOFTRENDER] -  building for WindowsStore....")
   add_definitions(-DON_UWP)
   set(ON_UWP ON)
   set(CMAKE_CXX_STANDARD 17)
    set(CMAKE_CXX_STANDARD_REQUIRED ON)
elseif(CMAKE_SYSTEM_NAME MATCHES Windows)
    message(INFO "[SOFTRENDER] - building for Windows....")
    add_definitions(-DON_WINDOWS)
    set(ON_WINDOWS ON)
    set(CMAKE_CXX_STANDARD 23)
    set(CMAKE_CXX_STANDARD_REQUIRED ON)
else()
    message(FATAL_ERROR "system not supported")
endif()

if (CMAKE_CXX_COMPILER_ID MATCHES Clang)
    add_definitions(-DIS_CLANG)
elseif (CMAKE_CXX_COMPILER_ID MATCHES GNU)
    if(CMAKE_C_COMPILER_VERSION VERSION_LESS 12)
        message(FATAL_ERROR "anything lower that gcc-12 compiler not supported")
    endif()
    add_definitions(-DIS_GCC)
elseif (CMAKE_CXX_COMPILER_ID MATCHES MSVC)
    add_definitions(-DIS_MSVC)
else()
    message(FATAL_ERROR "compiler not supported")
endif()

add_subdirectory(dependances)
add_executable(${CMAKE_PROJECT_NAME}  ${PROJECT_SOURCE_DIR}/src/Main.cpp)
flagsSetup()
find_package(OpenGL REQUIRED)
find_package(OpenMP REQUIRED)

target_link_libraries(${CMAKE_PROJECT_NAME} glfw ${OPENGL_opengl_LIBRARY} OpenMP::OpenMP_CXX meshoptimizer)
target_include_directories(${CMAKE_PROJECT_NAME} PRIVATE ${OPENGL_INCLUDE_DIR})
target_include_directories(${CMAKE_PROJECT_NAME} PRIVATE ${PROJECT_SOURCE_DIR}/src STATIC)
target_include_directories(${CMAKE_PROJECT_NAME} PRIVATE ${PROJECT_SOURCE_DIR}/src/thirdparty STATIC)
target_include_directories(${CMAKE_PROJECT_NAME} PRIVATE ${PROJECT_SOURCE_DIR}src/thirdparty/meshoptimizer/src STATIC)
target_include_directories(${CMAKE_PROJECT_NAME} PRIVATE ${PROJECT_SOURCE_DIR}/dependances/glfw/include STATIC)
target_include_directories(${CMAKE_PROJECT_NAME} PRIVATE ${PROJECT_SOURCE_DIR}/dependances/glm STATIC)
include(SourceList)
add_subdirectory(${PROJECT_SOURCE_DIR}/src)