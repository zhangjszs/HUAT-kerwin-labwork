cmake_minimum_required(VERSION 3.10)

# 项目名称
project(VehicleClaimManagement)

# 设置 C 标准
set(CMAKE_C_STANDARD 99)

# 添加源文件
set(SOURCE_FILES
    vehicle_claim.c
)

# 添加头文件路径
include_directories(${CMAKE_SOURCE_DIR})

# 生成可执行文件
add_executable(VehicleClaimManagement ${SOURCE_FILES})