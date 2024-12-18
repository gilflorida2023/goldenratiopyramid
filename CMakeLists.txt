
cmake_minimum_required(VERSION 3.10)
project(goldenratiopyramid)

# Find SDL2 and SDL2_image packages
find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)

# Add the executable
add_executable(goldenratiopyramid goldenratiopyramid.c)

# Link libraries
target_link_libraries(goldenratiopyramid
    ${SDL2_LIBRARIES}
    SDL2_image::SDL2_image
    m
)

# Set compiler flags if needed
target_compile_options(goldenratiopyramid PRIVATE -O0 -g3)

