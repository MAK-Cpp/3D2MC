// Include standard headers
#include <cmath>
#include <iostream>
#include <random>

// Include GLEW
#include <GL/glew.h>

// Include GLFW
#include <GLFW/glfw3.h>

// Include Graphic Library Math
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>

#include "shader.hpp"

// Need to get variables from CMake
#include "config.h"

glm::vec3 vectorMultiplication(const glm::vec3& a, const glm::vec3& b) {
    return glm::vec3(a[1] * b[2] - a[2] * b[1], a[2] * b[0] - a[0] * b[2],
                     a[0] * b[1] - a[1] * b[0]);
}

glm::vec3 vec4to3 (const glm::vec4& to_3) {
    return glm::vec3(to_3[0], to_3[1], to_3[2]);
}

glm::vec4 vec3to4 (const glm::vec3& to_4, const float end = 0) {
    return glm::vec4(to_4[0], to_4[1], to_4[2], end);
}

glm::vec3 rotateVec3(const glm::vec3& vec_to_rotate, glm::f32 angle, const glm::highp_vec3 &axis, const float end = 0) {
    glm::vec4 result = glm::rotate(angle, axis) * vec3to4(vec_to_rotate, end);
    return vec4to3(result);
}



int main(void) {
    constexpr int kWidth = 800;
    constexpr int kHeight = 600;
    constexpr float kCamDegrees = 45;
    constexpr float kRotationRadians = glm::radians(2.5f);
    const float kCosRot = std::cos(kRotationRadians);
    const float kSinRot = std::sin(kRotationRadians);
    const glm::mat4 kIdentityMatrix(1);

    double mouse_position_x_begin;
    double mouse_position_y_begin;
    double mouse_position_x_end;
    double mouse_position_y_end;

    glm::mat4 projection_matrix = glm::perspective(
        glm::radians(
            kCamDegrees),  // Вертикальное поле зрения в радианах. Обычно между
                           // 90&deg; (очень широкое) и 30&deg; (узкое)
        static_cast<float>(kWidth) /
            static_cast<float>(
                kHeight),  // Отношение сторон. Зависит от размеров вашего окна.
                           // Заметьте, что 4/3 == 800/600 == 1280/960
        0.1f,  // Ближняя плоскость отсечения. Должна быть больше 0.
        100.0f  // Дальняя плоскость отсечения.
    );

    glm::mat4 model(1.0f);

    // Initialise GLFW
    if (!glfwInit()) {
        fprintf(stderr, "Failed to initialize GLFW\n");
        getchar();
        return -1;
    }

    glfwWindowHint(GLFW_SAMPLES, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Open a window and create its OpenGL context
    GLFWwindow* window;
    window =
        glfwCreateWindow(kWidth, kHeight, "Tutorial 001", nullptr, nullptr);

    if (window == nullptr) {
        fprintf(
            stderr,
            "Failed to open GLFW window. If you have an Intel GPU, they are "
            "not 3.3 compatible. Try the 2.1 version of the tutorials.\n");
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    // Initialize GLEW
    if (glewInit() != GLEW_OK) {
        fprintf(stderr, "Failed to initialize GLEW\n");
        getchar();
        glfwTerminate();
        return -1;
    }

    // Ensure we can capture the escape key being pressed below
    glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

    // Dark blue background
    glClearColor(0.0f, 0.0f, 0.5f, 0.0f);

    // Enable depth test
    glEnable(GL_DEPTH_TEST);
    // Accept fragment if it closer to the camera than the former one
    glDepthFunc(GL_LESS);

    // Vertex Array Object
    GLuint VertexArrayID;
    glGenVertexArrays(1, &VertexArrayID);
    glBindVertexArray(VertexArrayID);

    // Our vertices. Three consecutive floats give a 3D vertex; Three
    // consecutive vertices give a triangle.
    // A cube has 6 faces with 2 triangles each, so this makes 6*2=12 triangles,
    // and 12*3 vertices
    static const GLfloat g_vertex_buffer_data[] = {
        -1.0f, -1.0f, -1.0f, -1.0f, -1.0f, 1.0f,  -1.0f, 1.0f,  1.0f,  1.0f,
        1.0f,  -1.0f, -1.0f, -1.0f, -1.0f, -1.0f, 1.0f,  -1.0f, 1.0f,  -1.0f,
        1.0f,  -1.0f, -1.0f, -1.0f, 1.0f,  -1.0f, -1.0f, 1.0f,  1.0f,  -1.0f,
        1.0f,  -1.0f, -1.0f, -1.0f, -1.0f, -1.0f, -1.0f, -1.0f, -1.0f, -1.0f,
        1.0f,  1.0f,  -1.0f, 1.0f,  -1.0f, 1.0f,  -1.0f, 1.0f,  -1.0f, -1.0f,
        1.0f,  -1.0f, -1.0f, -1.0f, -1.0f, 1.0f,  1.0f,  -1.0f, -1.0f, 1.0f,
        1.0f,  -1.0f, 1.0f,  1.0f,  1.0f,  1.0f,  1.0f,  -1.0f, -1.0f, 1.0f,
        1.0f,  -1.0f, 1.0f,  -1.0f, -1.0f, 1.0f,  1.0f,  1.0f,  1.0f,  -1.0f,
        1.0f,  1.0f,  1.0f,  1.0f,  1.0f,  1.0f,  -1.0f, -1.0f, 1.0f,  -1.0f,
        1.0f,  1.0f,  1.0f,  -1.0f, 1.0f,  -1.0f, -1.0f, 1.0f,  1.0f,  1.0f,
        1.0f,  1.0f,  -1.0f, 1.0f,  1.0f,  1.0f,  -1.0f, 1.0f};

    static const GLfloat g_color_buffer_data[] = {
        0.583f, 0.771f, 0.014f, 0.609f, 0.115f, 0.436f, 0.327f, 0.483f, 0.844f,
        0.822f, 0.569f, 0.201f, 0.435f, 0.602f, 0.223f, 0.310f, 0.747f, 0.185f,
        0.597f, 0.770f, 0.761f, 0.559f, 0.436f, 0.730f, 0.359f, 0.583f, 0.152f,
        0.483f, 0.596f, 0.789f, 0.559f, 0.861f, 0.639f, 0.195f, 0.548f, 0.859f,
        0.014f, 0.184f, 0.576f, 0.771f, 0.328f, 0.970f, 0.406f, 0.615f, 0.116f,
        0.676f, 0.977f, 0.133f, 0.971f, 0.572f, 0.833f, 0.140f, 0.616f, 0.489f,
        0.997f, 0.513f, 0.064f, 0.945f, 0.719f, 0.592f, 0.543f, 0.021f, 0.978f,
        0.279f, 0.317f, 0.505f, 0.167f, 0.620f, 0.077f, 0.347f, 0.857f, 0.137f,
        0.055f, 0.953f, 0.042f, 0.714f, 0.505f, 0.345f, 0.783f, 0.290f, 0.734f,
        0.722f, 0.645f, 0.174f, 0.302f, 0.455f, 0.848f, 0.225f, 0.587f, 0.040f,
        0.517f, 0.713f, 0.338f, 0.053f, 0.959f, 0.120f, 0.393f, 0.621f, 0.362f,
        0.673f, 0.211f, 0.457f, 0.820f, 0.883f, 0.371f, 0.982f, 0.099f, 0.879f};

    // This will identify our vertex buffer
    GLuint vertexbuffer;
    // Generate 1 buffer, put the resulting identifier in vertexbuffer
    glGenBuffers(1, &vertexbuffer);
    // The following commands will talk about our 'vertexbuffer' buffer
    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
    // Give our vertices to OpenGL.
    glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data),
                 g_vertex_buffer_data, GL_STATIC_DRAW);

    GLuint colorbuffer;
    glGenBuffers(1, &colorbuffer);
    glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(g_color_buffer_data),
                 g_color_buffer_data, GL_STATIC_DRAW);

    // Create and compile our GLSL program from the shaders

    GLuint programID =
        LoadShaders(PROJECT_DIR / "shaders/TransformVertexShader.vertexshader",
                    PROJECT_DIR / "shaders/ColorFragmentShader.fragmentshader");

    glm::vec4 camera_position(4, 3, 3, 1);
    glm::vec3 camera_center(0, 0, 0);
    glm::vec4 head(0, 1, 0, 0);
    glm::vec3 Axis(std::rand(), std::rand(), std::rand());

    glm::mat3 rotation(kCosRot, 0, -kSinRot, 0, 1, 0, kSinRot, 0, kCosRot);

    glfwGetCursorPos(window, &mouse_position_x_end, &mouse_position_y_end);

    do {
        glm::vec3 front = vec4to3(camera_position) - camera_center;
        glm::vec3 side = vectorMultiplication(front, head);
        mouse_position_x_begin = mouse_position_x_end;
        mouse_position_y_begin = mouse_position_y_end;
        glfwGetCursorPos(window, &mouse_position_x_end, &mouse_position_y_end);
        if ((mouse_position_x_begin != mouse_position_x_end ||
             mouse_position_y_begin != mouse_position_y_end) &&
            glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
            Axis = vectorMultiplication(vec4to3(camera_position), vec4to3(head));

            glm::vec2 mouse_vector = glm::normalize(
                glm::vec2(mouse_position_x_end - mouse_position_x_begin,
                          mouse_position_y_end - mouse_position_y_begin));

            Axis = rotateVec3(Axis, std::acos(mouse_vector[1]) * (mouse_vector[0] >= 0 ? 1 : -1), vec4to3(camera_position));
            camera_position = glm::rotate(kIdentityMatrix, kRotationRadians, Axis) * camera_position;
            head = glm::rotate(kIdentityMatrix, kRotationRadians, Axis) * head;

            camera_center = rotateVec3(camera_center, kRotationRadians, Axis);

        }
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
            camera_position += vec3to4(glm::normalize(side)) / 10.0f;
            camera_center += glm::normalize(side) / 10.0f;
        }
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
            camera_position -= vec3to4(glm::normalize(side)) / 10.0f;
            camera_center -= glm::normalize(side) / 10.0f;
        }
        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
            camera_position -= vec3to4(glm::normalize(front)) / 10.0f;
            camera_center -= glm::normalize(front) / 10.0f;
        }
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
            camera_position += vec3to4(glm::normalize(front)) / 10.0f;
            camera_center += glm::normalize(front) / 10.0f;
        }
        if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
            camera_position += glm::normalize(head) / 10.0f;
            camera_center += vec4to3(glm::normalize(head)) / 10.0f;
        }
        if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS) {
            camera_position -= glm::normalize(head) / 10.0f;
            camera_center -= vec4to3(glm::normalize(head)) / 10.0f;
        }
        // if (glfwGetCursorPos(window, GLFW_MOUSE_))
        glm::mat4 view = glm::lookAt(
            vec4to3(camera_position),  // Камера находится в мировых
                                            // координатах (4,3,3)
            camera_center,  // И направлена в начало координат
            vec4to3(head)  // "Голова" находится сверху
        );

        glm::mat4 MVP = projection_matrix * view * model;

        GLuint MatrixID = glGetUniformLocation(programID, "MVP");

        glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);

        // Clear the screen. It's not mentioned before Tutorial 02, but it can
        // cause flickering, so it's there nonetheless.
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Use our shader
        glUseProgram(programID);
        // Draw triangle...

        // 1st attribute buffer : vertices
        glEnableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
        glVertexAttribPointer(0,  // attribute 0. No particular reason for 0,
                                  // but must match the layout in the shader.
                              3,         // size
                              GL_FLOAT,  // type
                              GL_FALSE,  // normalized?
                              0,         // stride
                              (void*)0   // array buffer offset
        );

        // 2nd attribute buffer : colors
        glEnableVertexAttribArray(1);
        glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
        glVertexAttribPointer(1,  // attribute. No particular reason for 1, but
                                  // must match the layout in the shader.
                              3,         // size
                              GL_FLOAT,  // type
                              GL_FALSE,  // normalized?
                              0,         // stride
                              (void*)0   // array buffer offset
        );

        // Draw the triangle !
        glDrawArrays(
            GL_TRIANGLES, 0,
            3 * 12);  // Starting from vertex 0; 3 vertices total -> 1 triangle
        glDisableVertexAttribArray(0);
        glDisableVertexAttribArray(1);

        // Swap buffers
        glfwSwapBuffers(window);
        glfwPollEvents();


    }  // Check if the ESC key was pressed or the window was closed
    while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS &&
           glfwWindowShouldClose(window) == 0);

    // Close OpenGL window and terminate GLFW
    glfwTerminate();

    return 0;
}
