add_executable(${PROJECT_NAME} programm.cpp)

target_link_libraries(${PROJECT_NAME} PUBLIC shader 
                                            controls 
                                            objloader 
                                            quaternion_utils 
                                            tangentspace 
                                            text2D 
                                            texture 
                                            vboindexer
                                            cube
                    )

target_include_directories(${PROJECT_NAME} PUBLIC
                          ${PROJECT_BINARY_DIR}/lib
                          ${PROJECT_BINARY_DIR}/lib/figures
                          ${PROJECT_BINARY_DIR}/shaders
                          )

if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
    target_link_libraries(${PROJECT_NAME} PRIVATE OpenGL32.lib glew32.lib libglfw3.a)
elseif("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MCVS")
    target_link_libraries(${PROJECT_NAME} PRIVATE OpenGL32.lib glew32.lib glfw3.lib)
endif()

