cmake_minimum_required(VERSION 3.10)
project(flutter_cpp_physics_engine)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Output directory for the shared library
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

# Include header files
include_directories(include)

# Create shared library
add_library(game_engine SHARED
    src/game_engine.cpp
)

# Optional: Rename output file depending on OS
if(WIN32)
    set_target_properties(game_engine PROPERTIES OUTPUT_NAME "game_engine")
elseif(APPLE)
    set_target_properties(game_engine PROPERTIES SUFFIX ".dylib")
else()
    set_target_properties(game_engine PROPERTIES SUFFIX ".so")
endif()

# next step is
# mkdir build
# cd build
# cmake ..
# cmake --build .
