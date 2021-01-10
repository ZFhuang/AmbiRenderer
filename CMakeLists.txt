cmake_minimum_required (VERSION 3.8)

# 项目名
project (AmbiRenderer)

# 添加存放文件的子目录, 第一个参数是源目录, 第二个是目标目录, 第三个参数是需要排除的目录
add_subdirectory (src bin)

# 也可以单独设置输出目录, 这里分别是设置了可执行文件的目录和库文件的目录
# 在哪里 ADD_EXECUTABLE 或 ADD_LIBRARY， 如果需要改变目标存放路径，就在哪里加入上述的定义。
SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)
SET(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib)

# 打印当前的项目路径信息
MESSAGE(STATUS "***This is BINARY dir " ${PROJECT_BINARY_DIR})
MESSAGE(STATUS "***This is SOURCE dir " ${PROJECT_SOURCE_DIR})