#include "cube.h"


figure::Cube::Cube(const float x, const float y, const float z)
    : model_(glm::translate(kIdentityMatrix, glm::vec3(x, y, z))) 
    , X(model_[3][0])
    , Y(model_[3][1])
    , Z(model_[3][2]) {
    glGenVertexArrays(1, &VertexArrayID_);
    glBindVertexArray(VertexArrayID_);

    glGenBuffers(1, &vertexbuffer_);
    // The following commands will talk about our 'vertexbuffer' buffer
    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer_);
    // Give our vertices to OpenGL.
    glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);

    glGenBuffers(1, &colorbuffer_);
    glBindBuffer(GL_ARRAY_BUFFER, colorbuffer_);
    glBufferData(GL_ARRAY_BUFFER, sizeof(g_color_buffer_data), g_color_buffer_data, GL_STATIC_DRAW);
}

figure::Cube::Cube(const glm::vec3& xyz)
    : model_(glm::translate(kIdentityMatrix, xyz))
    , X(model_[3][0])
    , Y(model_[3][1])
    , Z(model_[3][2]) {
    glGenVertexArrays(1, &VertexArrayID_);
    glBindVertexArray(VertexArrayID_);

    // glm::translate()

    glGenBuffers(1, &vertexbuffer_);
    // The following commands will talk about our 'vertexbuffer' buffer
    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer_);
    // Give our vertices to OpenGL.
    glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);

    glGenBuffers(1, &colorbuffer_);
    glBindBuffer(GL_ARRAY_BUFFER, colorbuffer_);
    glBufferData(GL_ARRAY_BUFFER, sizeof(g_color_buffer_data), g_color_buffer_data, GL_STATIC_DRAW);
}

figure::Cube::Cube(const figure::Cube& other)
    : model_(other.model_)
    , X(model_[3][0])
    , Y(model_[3][1])
    , Z(model_[3][2]) {
    glGenVertexArrays(1, &VertexArrayID_);
    glBindVertexArray(VertexArrayID_);

    // glm::translate()

    glGenBuffers(1, &vertexbuffer_);
    // The following commands will talk about our 'vertexbuffer' buffer
    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer_);
    // Give our vertices to OpenGL.
    glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);

    glGenBuffers(1, &colorbuffer_);
    glBindBuffer(GL_ARRAY_BUFFER, colorbuffer_);
    glBufferData(GL_ARRAY_BUFFER, sizeof(g_color_buffer_data), g_color_buffer_data, GL_STATIC_DRAW);

}

figure::Cube::~Cube() {
    glDeleteVertexArrays(1, &VertexArrayID_);
    glDeleteBuffers(1, &vertexbuffer_);
    glDeleteBuffers(1, &colorbuffer_);
}


figure::Cube& figure::Cube::operator=(const figure::Cube& other) {
    model_ = other.model_;
    X = model_[3][0];
    Y = model_[3][1];
    Z = model_[3][2];

    return *this;
}

const glm::mat4& figure::Cube::model() const {
    return model_;
}

void figure::Cube::Draw() const {
    // 1st attribute buffer : vertices
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer_);
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
    glBindBuffer(GL_ARRAY_BUFFER, colorbuffer_);
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
}