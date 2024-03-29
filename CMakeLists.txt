cmake_minimum_required(VERSION 3.12)

project(3D2MC VERSION 0.0.1 LANGUAGES CXX)

# need for linux to find GLFW3
list(APPEND CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake")
message("\nNEW VALUE IN CMAKE_MODULE_PATH IS ${PROJECT_SOURCE_DIR}/cmake\n")

find_package(OpenGL 3.3 REQUIRED)
find_package(GLEW REQUIRED)
find_package(glm REQUIRED)
find_package(glfw3 3.3 REQUIRED)



set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# set(CMAKE_CXX_CLANG_TIDY clang-tidy)
# set(CMAKE_CXX_CLANG_TIDY "clang-tidy;-checks=*")


set(GRAPHIC_LIBRARIES glfw GLEW::glew OpenGL::GL)

if (APPLE OR LINUX)
    set(LIBRARIES ${GRAPHIC_LIBRARIES} glm::glm)
else ()
    set(LIBRARIES ${GRAPHIC_LIBRARIES} glm)
endif()
link_libraries(${GRAPHIC_LIBRARIES})

# Configure clang-tidy
if (${USE_CLANG_TIDY})
    set(CMAKE_CXX_CLANG_TIDY clang-tidy)
endif()

add_compile_definitions(-DPROJECT_SOURCE_DIR="${PROJECT_SOURCE_DIR}")
add_subdirectory(lib)
add_subdirectory(src)

#target_compile_options(${PROJECT_NAME} PRIVATE ${COMPILE_OPT})





