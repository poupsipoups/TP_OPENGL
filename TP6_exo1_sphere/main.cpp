#include <glimac/sphere_vertices.hpp>
#include <iostream>
#include "glimac/common.hpp"
#include "glm/ext/matrix_clip_space.hpp"
#include "glm/ext/matrix_transform.hpp"
#include "glm/fwd.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "glm/matrix.hpp"
#include "p6/p6.h"

int main()
{
    auto ctx = p6::Context{{1280, 720, "TP3 EX2"}};
    ctx.maximize_window();

    /*********************************
     * HERE SHOULD COME THE INITIALIZATION CODE
     *********************************/

    // On charge les shaders
    const p6::Shader shader = p6::load_shader("shaders/3D.vs.glsl", "shaders/normals.fs.glsl");

    shader.use();

    // Variables uniformes
    GLint uMVPMatrix_location    = glGetUniformLocation(shader.id(), "uMVPMatrix");
    GLint uMVMatrix_location     = glGetUniformLocation(shader.id(), "UMVMatrix");
    GLint uNormalMatrix_location = glGetUniformLocation(shader.id(), "uNormalMatrix");

    glEnable(GL_DEPTH_TEST);

    glm::mat4 ProjMatrix   = glm::perspective(glm::radians(70.f), ctx.aspect_ratio(), 0.1f, 100.f);
    glm::mat4 MVMatrix     = glm::translate(glm::mat4(1), glm::vec3(0, 0, -5));
    glm::mat4 NormalMatrix = glm::transpose(glm::inverse(MVMatrix));

    // Vertices de la sphere
    const std::vector<glimac::ShapeVertex> vertices = glimac::sphere_vertices(1.f, 32, 16);

    // Creation du VBO
    GLuint vbo;
    glGenBuffers(1, &vbo);

    // Bind du VBO sur la cible
    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    // On envoie les données du vbo
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glimac::ShapeVertex), vertices.data(), GL_STATIC_DRAW);

    // Debind du VBO (on evite des modifications involontaires)
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // Creation du VAO;
    GLuint vao;
    glGenVertexArrays(1, &vao);

    // Bind le VAO
    glBindVertexArray(vao);

    // On dit à opengl qu'on bind le vao
    const GLuint VERTEX_ATTR_POSITION   = 0;
    const GLuint VERTEX_ATTR_NORMAL     = 1;
    const GLuint VERTEX_ATTR_TEXTCOORDS = 2;
    glEnableVertexAttribArray(VERTEX_ATTR_POSITION);
    glEnableVertexAttribArray(VERTEX_ATTR_NORMAL);
    glEnableVertexAttribArray(VERTEX_ATTR_TEXTCOORDS);

    // on indique à openGL où trouver les sommets
    // bind le vbo
    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    // on indique le format de l'attribut de sommet position
    glVertexAttribPointer(VERTEX_ATTR_POSITION, 3, GL_FLOAT, GL_FALSE, sizeof(glimac::ShapeVertex), (const GLvoid*)offsetof(glimac::ShapeVertex, position));

    glVertexAttribPointer(VERTEX_ATTR_NORMAL, 3, GL_FLOAT, GL_FALSE, sizeof(glimac::ShapeVertex), (const GLvoid*)offsetof(glimac::ShapeVertex, normal));

    glVertexAttribPointer(VERTEX_ATTR_TEXTCOORDS, 2, GL_FLOAT, GL_FALSE, sizeof(glimac::ShapeVertex), (const GLvoid*)offsetof(glimac::ShapeVertex, texCoords));

    // debind du VBO
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // debind du VAO
    glBindVertexArray(0);

    // Declare your infinite update loop.
    ctx.update = [&]() {
        /*********************************
         * HERE SHOULD COME THE RENDERING CODE
         *********************************/

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        shader.use();

        glBindVertexArray(vao);
        glUniformMatrix4fv(uMVPMatrix_location, 1, GL_FALSE, glm::value_ptr(ProjMatrix * MVMatrix));
        glUniformMatrix4fv(uMVMatrix_location, 1, GL_FALSE, glm::value_ptr(MVMatrix));
        glUniformMatrix4fv(uNormalMatrix_location, 1, GL_FALSE, glm::value_ptr(NormalMatrix));

        glDrawArrays(GL_TRIANGLES, 0, vertices.size());

        glBindVertexArray(0);
    };

    // Should be done last. It starts the infinite loop.
    ctx.start();
    glDeleteBuffers(1, &vbo);
    glDeleteVertexArrays(1, &vao);
}