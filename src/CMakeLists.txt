add_executable(${PROJECT_NAME} programm.cpp)

target_link_libraries(${PROJECT_NAME} PUBLIC ${GRAPHIC_LIBRARIES})
target_include_directories(${PROJECT_NAME} PUBLIC
                          ${PROJECT_BINARY_DIR}/lib
                          )

if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
    target_link_libraries(${PROJECT_NAME} PRIVATE OpenGL32.lib glew32.lib libglfw3.a)
elseif("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MCVS")
    target_link_libraries(${PROJECT_NAME} PRIVATE OpenGL32.lib glew32.lib glfw3.lib)
endif()

