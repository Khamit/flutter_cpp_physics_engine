cmake_minimum_required(VERSION 3.4.1)
project(game_engine)

# Paths
set(SRC_DIR src)
set(INCLUDE_DIR ${SRC_DIR}/include)

# Source files
add_library(game_engine SHARED
    ${SRC_DIR}/game_engine.cpp
    ${SRC_DIR}/BoardManager.cpp
    ${SRC_DIR}/PhysicsBody.cpp
    ${SRC_DIR}/PhysicsWorld.cpp
    ${SRC_DIR}/Vector2.cpp
    ${SRC_DIR}/jewel.cpp
    ${SRC_DIR}/InitialBoardGenerator.cpp
)

# Include headers
target_include_directories(game_engine PRIVATE ${INCLUDE_DIR})

# Link Android log library
find_library(log-lib log)
target_link_libraries(game_engine ${log-lib})
