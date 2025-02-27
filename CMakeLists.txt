cmake_minimum_required(VERSION 3.10)

project(sleep_benchmark VERSION 1.0)

# 设置 C++ 标准
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 构建类型默认为Debug
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Debug")
endif()

# 源文件
file(GLOB_RECURSE srcs "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")

# 创建可执行文件
add_executable(${PROJECT_NAME} ${srcs})

# 设置输出目录
set(bin_dir ${CMAKE_CURRENT_BINARY_DIR}/bin)
set_target_properties(${PROJECT_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${bin_dir})
set_target_properties(${PROJECT_NAME} PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${bin_dir})
set_target_properties(${PROJECT_NAME} PROPERTIES ARCHIVE_OUTPUT_DIRECTORY ${bin_dir})
set_target_properties(${PROJECT_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY_DEBUG ${bin_dir})
set_target_properties(${PROJECT_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY_RELEASE ${bin_dir})
set_target_properties(${PROJECT_NAME} PROPERTIES LIBRARY_OUTPUT_DIRECTORY_DEBUG ${bin_dir})
set_target_properties(${PROJECT_NAME} PROPERTIES LIBRARY_OUTPUT_DIRECTORY_RELEASE ${bin_dir})
set_target_properties(${PROJECT_NAME} PROPERTIES ARCHIVE_OUTPUT_DIRECTORY_DEBUG ${bin_dir})
set_target_properties(${PROJECT_NAME} PROPERTIES ARCHIVE_OUTPUT_DIRECTORY_RELEASE ${bin_dir})

# 获取系统和架构类型
set(os_name "debian")
set(os_version "12")
set(os_arch "${CMAKE_SYSTEM_PROCESSOR}")

if(WIN32)
    set(os_name "windows")
    set(os_version "msvc")
    set(os_arch "x86_64")
else()
    if(EXISTS "/etc/os-release")
        file(READ "/etc/os-release" os_release_content)
        string(REGEX MATCH "ID=\"([^\"]*)\"" _ ${os_release_content})
        set(os_name ${CMAKE_MATCH_1})
        string(REGEX MATCH "VERSION_ID=\"([^\"]*)\"" _ ${os_release_content})
        set(os_version ${CMAKE_MATCH_1})
    endif()
endif()

message(STATUS "os name=${os_name}, os version=${os_version}, os arch=${os_arch}, build type=${CMAKE_BUILD_TYPE}")

# 指定目标的包含目录
target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# 系统自定义参数
if(os_name MATCHES "windows")
    target_link_libraries(${PROJECT_NAME} PRIVATE ws2_32)
else()
    target_link_libraries(${PROJECT_NAME} PRIVATE pthread)
endif()
