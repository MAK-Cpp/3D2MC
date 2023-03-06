// Include standard headers
#include <cmath>
#include <iostream>
#include <random>
#include <vector>

// Include GLEW
#include <GL/glew.h>

// Include GLFW
#include <GLFW/glfw3.h>

// Include Graphic Library Math
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>

#include "shader.hpp"

// figures
#include "figures/cube.h"

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


std::ostream& operator<< (std::ostream& op, const glm::mat4& mat) {
    for (int x = 0; x < 4; x++) {
        for (int y = 0; y < 4; y++) {
            op << mat[x][y] << ' ';
        }
        op << '\n';
    }
    return op;
}


int main(void) {
    // Initialise GLFW
    if (!glfwInit()) {
        fprintf(stderr, "Failed to initialize GLFW\n");
        getchar();
        return -1;
    }

    constexpr int kWidth = 1920;
    constexpr int kHeight = 1080;
    constexpr float kCamDegrees = 45;
    constexpr float kRotationRadians = glm::radians(2.5f);
    // const float kCosRot = std::cos(kRotationRadians);
    // const float kSinRot = std::sin(kRotationRadians);

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

    glfwWindowHint(GLFW_SAMPLES, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Open a window and create its OpenGL context
    GLFWwindow* window = glfwCreateWindow(kWidth, kHeight, "Tutorial 001", nullptr, nullptr);

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


    GLuint VertexArrayID;
    glGenVertexArrays(1, &VertexArrayID);
    glBindVertexArray(VertexArrayID);

    // Ensure we can capture the escape key being pressed below
    glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

    // Dark blue background
    glClearColor(0.0f, 0.0f, 0.5f, 0.0f);

    // Enable depth test
    glEnable(GL_DEPTH_TEST);
    // Accept fragment if it closer to the camera than the former one
    glDepthFunc(GL_LESS);
    
    // Create and compile our GLSL program from the shaders

    GLuint programID =
        LoadShaders(PROJECT_DIR / "shaders/TransformVertexShader.vertexshader",
                    PROJECT_DIR / "shaders/ColorFragmentShader.fragmentshader");

    glm::vec4 camera_position(4, 3, 3, 1);
    glm::vec3 camera_center(0, 0, 0);
    glm::vec4 head(0, 1, 0, 0);
    glm::vec3 Axis(std::rand(), std::rand(), std::rand());


    constexpr int cnt = 5;
    std::vector <figure::Cube> blocks(5);
    for (int i = 0; i < cnt; i++) {
        blocks[i] = *new figure::Cube(i, i, i);
    }
    

    // Get a handle for our "MVP" uniform
    // Only during the initialisation
    GLuint MatrixID = glGetUniformLocation(programID, "MVP");


    glfwGetCursorPos(window, &mouse_position_x_end, &mouse_position_y_end);
    do {
        blocks[0].X += 0.01;
        glm::vec3 front = vec4to3(camera_position) - camera_center;
        glm::vec3 side = vectorMultiplication(front, head);
        mouse_position_x_begin = mouse_position_x_end;
        mouse_position_y_begin = mouse_position_y_end;
        glfwGetCursorPos(window, &mouse_position_x_end, &mouse_position_y_end);
        if ((mouse_position_x_begin != mouse_position_x_end ||
             mouse_position_y_begin != mouse_position_y_end) &&
            glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
            Axis = vectorMultiplication(vec4to3(camera_position), vec4to3(head));
            // Axis = side;

            glm::vec2 mouse_vector = glm::normalize(
                glm::vec2(mouse_position_x_end - mouse_position_x_begin,
                          mouse_position_y_end - mouse_position_y_begin));

            Axis = rotateVec3(Axis, std::acos(mouse_vector[1]) * (mouse_vector[0] >= 0 ? 1 : -1), vec4to3(camera_position));
            camera_position = glm::rotate(kIdentityMatrix, kRotationRadians, Axis) * camera_position;
            head = glm::rotate(kIdentityMatrix, kRotationRadians, Axis) * head;

            camera_center = rotateVec3(camera_center, kRotationRadians, Axis);

        }
        if ((mouse_position_x_begin != mouse_position_x_end ||
             mouse_position_y_begin != mouse_position_y_end) &&
            glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS) {
            Axis = side;

            glm::vec2 mouse_vector = glm::normalize(
                glm::vec2(mouse_position_x_end - mouse_position_x_begin,
                          mouse_position_y_end - mouse_position_y_begin));

            Axis = rotateVec3(Axis, std::acos(mouse_vector[1]) * (mouse_vector[0] >= 0 ? 1 : -1), vec4to3(camera_position));
            camera_position = glm::rotate(kIdentityMatrix, kRotationRadians, Axis) * camera_position;
            head = glm::rotate(kIdentityMatrix, kRotationRadians, Axis) * head;

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
        glm::mat4 view = glm::lookAt(
            vec4to3(camera_position),  // Камера находится в мировых
                                            // координатах (4,3,3)
            camera_center,  // И направлена в начало координат
            vec4to3(head)  // "Голова" находится сверху
        );

        // Clear the screen. It's not mentioned before Tutorial 02, but it can
            // cause flickering, so it's there nonetheless.
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    

        for (int i = 0; i < cnt; i++) {
            glm::mat4 MVP = projection_matrix * view * blocks[i].model();
            
            glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);

            

            // Use our shader
            glUseProgram(programID);
            // Draw triangle...

            blocks[i].Draw();
        }

       

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
