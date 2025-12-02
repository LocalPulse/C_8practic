cmake_minimum_required(VERSION 3.16)
project(8practic LANGUAGES C)
set(CMAKE_C_STANDARD 17)

set(SRCS 
    src/main.c
    src/services/game-service.c
    src/models/game.c
    src/utils/console.c
)

add_executable(${PROJECT_NAME} ${SRCS})
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/src)
